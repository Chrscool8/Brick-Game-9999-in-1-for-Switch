#include <cstdint>
#include <deko3d.hpp>
#include <optional>
#include <nanovg_dk.h>
#include <string>
#include <platform/switch/graphics_layer_switch.h>

graph_lib GL;

static int nxlink_sock = -1;

extern "C" void userAppInit(void)
{
	romfsInit();
	socketInitialize(NULL);
	nxlink_sock = nxlinkConnectToHost(true, true);
}

extern "C" void userAppExit(void)
{
	if (nxlink_sock != -1)
		close(nxlink_sock);
	socketExit();
	romfsExit();
}

void OutputDkDebug(void* userData, const char* context, DkResult result, const char* message)
{
	printf("Context: %s\nResult: %d\nMessage: %s\n", context, result, message);
}

void createFramebufferResources()
{
	// Create layout for the depth buffer
	dk::ImageLayout layout_depthbuffer;
	dk::ImageLayoutMaker{ GL.device }
		.setFlags(DkImageFlags_UsageRender | DkImageFlags_HwCompression)
		.setFormat(DkImageFormat_S8)
		.setDimensions(GL.FramebufferWidth, GL.FramebufferHeight)
		.initialize(layout_depthbuffer);

	// Create the depth buffer
	GL.depthBuffer_mem = GL.pool_images->allocate(layout_depthbuffer.getSize(), layout_depthbuffer.getAlignment());
	GL.depthBuffer.initialize(layout_depthbuffer, GL.depthBuffer_mem.getMemBlock(), GL.depthBuffer_mem.getOffset());

	// Create layout for the framebuffers
	dk::ImageLayout layout_framebuffer;
	dk::ImageLayoutMaker{ GL.device }
		.setFlags(DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_HwCompression)
		.setFormat(DkImageFormat_RGBA8_Unorm)
		.setDimensions(GL.FramebufferWidth, GL.FramebufferHeight)
		.initialize(layout_framebuffer);

	// Create the framebuffers
	std::array<DkImage const*, GL.NumFramebuffers> fb_array;
	uint64_t fb_size = layout_framebuffer.getSize();
	uint32_t fb_align = layout_framebuffer.getAlignment();
	for (unsigned i = 0; i < GL.NumFramebuffers; i++)
	{
		// Allocate a framebuffer
		GL.framebuffers_mem[i] = GL.pool_images->allocate(fb_size, fb_align);
		GL.framebuffers[i].initialize(layout_framebuffer, GL.framebuffers_mem[i].getMemBlock(), GL.framebuffers_mem[i].getOffset());

		// Generate a command list that binds it
		dk::ImageView colorTarget{ GL.framebuffers[i] }, depthTarget{ GL.depthBuffer };
		GL.cmdbuf.bindRenderTargets(&colorTarget, &depthTarget);
		GL.framebuffer_cmdlists[i] = GL.cmdbuf.finishList();

		// Fill in the array for use later by the swapchain creation code
		fb_array[i] = &GL.framebuffers[i];
	}

	// Create the swapchain using the framebuffers
	GL.swapchain = dk::SwapchainMaker{ GL.device, nwindowGetDefault(), fb_array }.create();

	// Generate the main rendering cmdlist
	recordStaticCommands();
}

void destroyFramebufferResources()
{
	// Return early if we have nothing to destroy
	if (!GL.swapchain) return;

	// Make sure the queue is idle before destroying anything
	GL.queue.waitIdle();

	// Clear the static cmdbuf, destroying the static cmdlists in the process
	GL.cmdbuf.clear();

	// Destroy the swapchain
	GL.swapchain.destroy();

	// Destroy the framebuffers
	for (unsigned i = 0; i < GL.NumFramebuffers; i++)
		GL.framebuffers_mem[i].destroy();

	// Destroy the depth buffer
	GL.depthBuffer_mem.destroy();
}

void recordStaticCommands()
{
	// Initialize state structs with deko3d defaults
	dk::RasterizerState rasterizerState;
	dk::ColorState colorState;
	dk::ColorWriteState colorWriteState;
	dk::BlendState blendState;

	// Configure the viewport and scissor
	GL.cmdbuf.setViewports(0, { { 0.0f, 0.0f, (float)GL.FramebufferWidth, (float)GL.FramebufferHeight, 0.0f, 1.0f } });
	GL.cmdbuf.setScissors(0, { { 0, 0, GL.FramebufferWidth, GL.FramebufferHeight } });

	// Clear the color and depth buffers
	GL.cmdbuf.clearColor(0, DkColorMask_RGBA, 109.f / 255.f, 120.f / 255.f, 92.f / 255.f, 1.0f);
	GL.cmdbuf.clearDepthStencil(true, 1.0f, 0xFF, 0);

	// Bind required state
	GL.cmdbuf.bindRasterizerState(rasterizerState);
	GL.cmdbuf.bindColorState(colorState);
	GL.cmdbuf.bindColorWriteState(colorWriteState);

	GL.render_cmdlist = GL.cmdbuf.finishList();
}

void initialize_graphics_switch(unsigned int width, unsigned int height)
{
	GL.FramebufferWidth = 1280;
	GL.FramebufferHeight = 720;
	GL.StaticCmdSize = 0x1000;

	// Create the deko3d device
	GL.device = dk::DeviceMaker{}.setCbDebug(OutputDkDebug).create();

	// Create the main queue
	GL.queue = dk::QueueMaker{ GL.device }.setFlags(DkQueueFlags_Graphics).create();

	// Create the memory pools
	GL.pool_images.emplace(GL.device, DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image, 16 * 1024 * 1024);
	GL.pool_code.emplace(GL.device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached | DkMemBlockFlags_Code, 128 * 1024);
	GL.pool_data.emplace(GL.device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached, 1 * 1024 * 1024);

	// Create the static command buffer and feed it freshly allocated memory
	GL.cmdbuf = dk::CmdBufMaker{ GL.device }.create();
	CMemPool::Handle cmdmem = GL.pool_data->allocate(GL.StaticCmdSize);
	GL.cmdbuf.addMemory(cmdmem.getMemBlock(), cmdmem.getOffset(), cmdmem.getSize());
	printf("cmdmem size: %u\n", cmdmem.getSize());
	// Create the framebuffer resources
	createFramebufferResources();

	GL.renderer.emplace(GL.FramebufferWidth, GL.FramebufferHeight, GL.device, GL.queue, *GL.pool_images, *GL.pool_code, *GL.pool_data);
	GL.vg = nvgCreateDk(&*GL.renderer, NVG_DEBUG);
}

void load_fonts_switch()
{
	int fontIcons = nvgCreateFont(GL.vg, "icons", "romfs:/fonts/entypo.ttf");
	if (fontIcons == -1) {
		printf("Could not add font icons.\n");
	}
	int fontNormal = nvgCreateFont(GL.vg, "sans", "romfs:/fonts/Roboto-Regular.ttf");
	if (fontNormal == -1) {
		printf("Could not add font italic.\n");
	}
	int fontBold = nvgCreateFont(GL.vg, "sans-bold", "romfs:/fonts/Roboto-Bold.ttf");
	if (fontBold == -1) {
		printf("Could not add font bold.\n");
	}
	int fontEmoji = nvgCreateFont(GL.vg, "emoji", "romfs:/fonts/NotoEmoji-Regular.ttf");
	if (fontEmoji == -1) {
		printf("Could not add font emoji.\n");
	}
	int fontSegment = nvgCreateFont(GL.vg, "seg", "romfs:/fonts/DSEG7Classic-Bold.ttf");
	if (fontSegment == -1) {
		printf("Could not add font segment.\n");
	}
	int fontMinecraft = nvgCreateFont(GL.vg, "minecraft", "romfs:/fonts/Minecraft.ttf");
	if (fontMinecraft == -1) {
		printf("Could not add font minecraft.\n");
	}
	int fontKongtext = nvgCreateFont(GL.vg, "kongtext", "romfs:/fonts/kongtext-regular.ttf");
	if (fontKongtext == -1) {
		printf("Could not add font Kongtext.\n");
	}
	int vcrtext = nvgCreateFont(GL.vg, "vcrtext", "romfs:/fonts/VCR_OSD_MONO_1.ttf");
	if (vcrtext == -1) {
		printf("Could not add font vcrtext.\n");
	}

	nvgAddFallbackFontId(GL.vg, fontNormal, fontEmoji);
	nvgAddFallbackFontId(GL.vg, fontBold, fontEmoji);
}

void exit_graphics_switch()
{
	// Destroy the framebuffer resources.This should be done first.
	destroyFramebufferResources();

	// Cleanup vg. This needs to be done first as it relies on the renderer.
	nvgDeleteDk(GL.vg);

	// Destroy the renderer
	GL.renderer.reset();
}

void push_graphics_switch()
{
	nvgSave(GL.vg);
}

void gfx_translate_switch(float x, float y)
{
	nvgTranslate(GL.vg, x, y);
}

void gfx_scale_switch(float x, float y)
{
	nvgScale(GL.vg, x, y);
}

void gfx_rotate_switch(float angle)
{
	nvgRotate(GL.vg, angle);
}

void pop_graphics_switch()
{
	nvgRestore(GL.vg);
}

void set_font_switch(std::string font_name)
{
	nvgFontFace(GL.vg, font_name.c_str());
}

void set_font_size_switch(float size)
{
	nvgFontSize(GL.vg, size);
}

void set_text_align_switch(int alignment)
{
	nvgTextAlign(GL.vg, alignment);
}

void gfx_set_fill_color_switch(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	nvgFillColor(GL.vg, nvgRGBA(r, g, b, a));
}

void draw_text_switch(float x, float y, std::string text)
{
	nvgText(GL.vg, x, y, text.c_str(), NULL);
}

std::map<std::string, int> sprite_indicies;

void load_sprite_switch(std::string sprite_name, std::string sprite_path)
{
	sprite_indicies[sprite_name] = nvgCreateImage(GL.vg, sprite_path.c_str(), NVG_IMAGE_NEAREST);
	if (sprite_indicies[sprite_name] == 0)
		printf(("Problem loading " + sprite_name + "\n").c_str());
	else
		printf(("Loaded " + sprite_name + " to index " + std::to_string(sprite_indicies[sprite_name]) + "\n").c_str());
}

bool draw_sprite_switch(float x, float y, float width, float height, std::string sprite_name)
{
	if (sprite_indicies.count(sprite_name) == 0)
	{
		printf(("Trying to draw unloaded sprite, " + sprite_name + "\n").c_str());
		return false;
	}
	else
	{
		//printf(("Trying to draw loaded sprite, " + sprite_name + "\n").c_str());
		nvgSave(GL.vg);
		nvgScissor(GL.vg, x, y, width, height);
		nvgTranslate(GL.vg, x, y);

		NVGpaint imgPaint = nvgImagePattern(GL.vg, 0, 0, width, height, 0.0f / 180.0f * NVG_PI, sprite_indicies[sprite_name], 1.0f);
		nvgBeginPath(GL.vg);
		nvgRect(GL.vg, 0, 0, width, height);
		nvgFillPaint(GL.vg, imgPaint);
		nvgFill(GL.vg);
		nvgRestore(GL.vg);
		return true;
	}
}

void gfx_start_frame_switch()
{
	// Acquire a framebuffer from the swapchain (and wait for it to be available)
	GL.slot = GL.queue.acquireImage(GL.swapchain);

	// Run the command list that attaches said framebuffer to the queue
	GL.queue.submitCommands(GL.framebuffer_cmdlists[GL.slot]);

	// Run the main rendering command list
	GL.queue.submitCommands(GL.render_cmdlist);

	nvgBeginFrame(GL.vg, GL.FramebufferWidth, GL.FramebufferHeight, 1.0f);
}

void gfx_end_frame_switch()
{
	nvgEndFrame(GL.vg);

	// Now that we are done rendering, present it to the screen
	GL.queue.presentImage(GL.swapchain, GL.slot);
}