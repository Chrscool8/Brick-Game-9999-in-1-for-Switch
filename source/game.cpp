#include <array>
#include <optional>
#include <unistd.h>
#include "perf.hpp"
#include <utils.hpp>
#include <grid.hpp>
#include <vector>
#include <game.h>
#include "nanovg.h"
#include "nanovg_dk.h"
#include <nanovg/framework/CMemPool.h>
#include <nanovg/framework/CApplication.h>
#include <switch/runtime/devices/socket.h>
#include <grid_sprites.h>
#include <game_snake.h>
#include <game_menu.h>
#include "audio.h"

static int nxlink_sock = -1;

using namespace std;

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

std::map<std::string, int> sprite_indicies;

void load_sprite(NVGcontext* vg, std::string sprite_name, std::string sprite_path)
{
	sprite_indicies[sprite_name] = nvgCreateImage(vg, sprite_path.c_str(), NVG_IMAGE_NEAREST);
	if (sprite_indicies[sprite_name] == 0)
		printf(("Problem loading " + sprite_name + "\n").c_str());
	else
		printf(("Loaded " + sprite_name + " to index " + std::to_string(sprite_indicies[sprite_name]) + "\n").c_str());
}

bool draw_sprite(NVGcontext* vg, float x, float y, float width, float height, std::string sprite_name)
{
	if (sprite_indicies.count(sprite_name) == 0)
	{
		printf(("Trying to draw unloaded sprite, " + sprite_name + "\n").c_str());
		return false;
	}
	else
	{
		//printf(("Trying to draw loaded sprite, " + sprite_name + "\n").c_str());
		nvgSave(vg);
		nvgScissor(vg, x, y, width, height);
		nvgTranslate(vg, x, y);

		NVGpaint  imgPaint = nvgImagePattern(vg, 0, 0, width, height, 0.0f / 180.0f * NVG_PI, sprite_indicies[sprite_name], 1.0f);
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, width, height);
		nvgFillPaint(vg, imgPaint);
		nvgFill(vg);
		nvgRestore(vg);
		return true;
	}
}

void OutputDkDebug(void* userData, const char* context, DkResult result, const char* message)
{
	printf("Context: %s\nResult: %d\nMessage: %s\n", context, result, message);
}

BrickGameFramework::BrickGameFramework()
{
	FramebufferWidth = 1280;
	FramebufferHeight = 720;
	StaticCmdSize = 0x1000;

	// Create the deko3d device
	device = dk::DeviceMaker{}.setCbDebug(OutputDkDebug).create();

	// Create the main queue
	queue = dk::QueueMaker{ device }.setFlags(DkQueueFlags_Graphics).create();

	// Create the memory pools
	pool_images.emplace(device, DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image, 16 * 1024 * 1024);
	pool_code.emplace(device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached | DkMemBlockFlags_Code, 128 * 1024);
	pool_data.emplace(device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached, 1 * 1024 * 1024);

	// Create the static command buffer and feed it freshly allocated memory
	cmdbuf = dk::CmdBufMaker{ device }.create();
	CMemPool::Handle cmdmem = pool_data->allocate(StaticCmdSize);
	cmdbuf.addMemory(cmdmem.getMemBlock(), cmdmem.getOffset(), cmdmem.getSize());

	// Create the framebuffer resources
	createFramebufferResources();

	this->renderer.emplace(FramebufferWidth, FramebufferHeight, this->device, this->queue, *this->pool_images, *this->pool_code, *this->pool_data);
	this->vg = nvgCreateDk(&*this->renderer, 0);

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	// Load Resources
	load_sprite(vg, "spr_cell_selected", "romfs:/images/cell_selected.png");
	load_sprite(vg, "spr_cell_selected_90", "romfs:/images/cell_selected_90.png");
	load_sprite(vg, "spr_cell_selected_180", "romfs:/images/cell_selected_180.png");
	load_sprite(vg, "spr_cell_selected_270", "romfs:/images/cell_selected_270.png");

	load_sprite(vg, "spr_cell_unselected", "romfs:/images/cell_unselected.png");
	load_sprite(vg, "spr_cell_unselected_90", "romfs:/images/cell_unselected_90.png");
	load_sprite(vg, "spr_cell_unselected_180", "romfs:/images/cell_unselected_180.png");
	load_sprite(vg, "spr_cell_unselected_270", "romfs:/images/cell_unselected_270.png");

	int fontIcons = nvgCreateFont(vg, "icons", "romfs:/fonts/entypo.ttf");
	if (fontIcons == -1) {
		printf("Could not add font icons.\n");
	}
	int fontNormal = nvgCreateFont(vg, "sans", "romfs:/fonts/Roboto-Regular.ttf");
	if (fontNormal == -1) {
		printf("Could not add font italic.\n");
	}
	int fontBold = nvgCreateFont(vg, "sans-bold", "romfs:/fonts/Roboto-Bold.ttf");
	if (fontBold == -1) {
		printf("Could not add font bold.\n");
	}
	int fontEmoji = nvgCreateFont(vg, "emoji", "romfs:/fonts/NotoEmoji-Regular.ttf");
	if (fontEmoji == -1) {
		printf("Could not add font emoji.\n");
	}
	nvgAddFallbackFontId(vg, fontNormal, fontEmoji);
	nvgAddFallbackFontId(vg, fontBold, fontEmoji);

	printf("Done loading\n");

	//

	current_game = -1;
	next_game = 0;
	screen_orientation = orientation_normal;

	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);

	game_grid = grid_create(10, 20);


	//
	game_item game_menu;
	game_menu.name = "Menu";
	game_menu.init_function = &game_menu_init;
	game_menu.run_function = &game_menu_run;
	game_menu.exit_function = &game_menu_exit;
	game_list.push_back(game_menu);

	//game_item game_snake;
	//game_snake.name = "Snake";
	//game_snake.init_function = &game_snake_init;
	//game_snake.run_function = &game_snake_run;
	//game_snake.exit_function = &game_snake_exit;
	//game_list.push_back(game_snake);

	subgame_snake snake;
	game_item game_snake;
	game_snake.name = "Snake";
	game_snake.init_function = snake.game_snake_init;
}

BrickGameFramework::~BrickGameFramework()
{
	// Destroy the framebuffer resources. This should be done first.
	destroyFramebufferResources();

	// Cleanup vg. This needs to be done first as it relies on the renderer.
	nvgDeleteDk(vg);

	// Destroy the renderer
	this->renderer.reset();
}

void BrickGameFramework::createFramebufferResources()
{
	// Create layout for the depth buffer
	dk::ImageLayout layout_depthbuffer;
	dk::ImageLayoutMaker{ device }
		.setFlags(DkImageFlags_UsageRender | DkImageFlags_HwCompression)
		.setFormat(DkImageFormat_S8)
		.setDimensions(FramebufferWidth, FramebufferHeight)
		.initialize(layout_depthbuffer);

	// Create the depth buffer
	depthBuffer_mem = pool_images->allocate(layout_depthbuffer.getSize(), layout_depthbuffer.getAlignment());
	depthBuffer.initialize(layout_depthbuffer, depthBuffer_mem.getMemBlock(), depthBuffer_mem.getOffset());

	// Create layout for the framebuffers
	dk::ImageLayout layout_framebuffer;
	dk::ImageLayoutMaker{ device }
		.setFlags(DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_HwCompression)
		.setFormat(DkImageFormat_RGBA8_Unorm)
		.setDimensions(FramebufferWidth, FramebufferHeight)
		.initialize(layout_framebuffer);

	// Create the framebuffers
	std::array<DkImage const*, NumFramebuffers> fb_array;
	uint64_t fb_size = layout_framebuffer.getSize();
	uint32_t fb_align = layout_framebuffer.getAlignment();
	for (unsigned i = 0; i < NumFramebuffers; i++)
	{
		// Allocate a framebuffer
		framebuffers_mem[i] = pool_images->allocate(fb_size, fb_align);
		framebuffers[i].initialize(layout_framebuffer, framebuffers_mem[i].getMemBlock(), framebuffers_mem[i].getOffset());

		// Generate a command list that binds it
		dk::ImageView colorTarget{ framebuffers[i] }, depthTarget{ depthBuffer };
		cmdbuf.bindRenderTargets(&colorTarget, &depthTarget);
		framebuffer_cmdlists[i] = cmdbuf.finishList();

		// Fill in the array for use later by the swapchain creation code
		fb_array[i] = &framebuffers[i];
	}

	// Create the swapchain using the framebuffers
	swapchain = dk::SwapchainMaker{ device, nwindowGetDefault(), fb_array }.create();

	// Generate the main rendering cmdlist
	recordStaticCommands();
}

void BrickGameFramework::destroyFramebufferResources()
{
	// Return early if we have nothing to destroy
	if (!swapchain) return;

	// Make sure the queue is idle before destroying anything
	queue.waitIdle();

	// Clear the static cmdbuf, destroying the static cmdlists in the process
	cmdbuf.clear();

	// Destroy the swapchain
	swapchain.destroy();

	// Destroy the framebuffers
	for (unsigned i = 0; i < NumFramebuffers; i++)
		framebuffers_mem[i].destroy();

	// Destroy the depth buffer
	depthBuffer_mem.destroy();
}

void BrickGameFramework::recordStaticCommands()
{
	// Initialize state structs with deko3d defaults
	dk::RasterizerState rasterizerState;
	dk::ColorState colorState;
	dk::ColorWriteState colorWriteState;
	dk::BlendState blendState;

	// Configure the viewport and scissor
	cmdbuf.setViewports(0, { { 0.0f, 0.0f, (float)FramebufferWidth, (float)FramebufferHeight, 0.0f, 1.0f } });
	cmdbuf.setScissors(0, { { 0, 0, FramebufferWidth, FramebufferHeight } });

	// Clear the color and depth buffers
	cmdbuf.clearColor(0, DkColorMask_RGBA, 109.f / 255.f, 120.f / 255.f, 92.f / 255.f, 1.0f);
	cmdbuf.clearDepthStencil(true, 1.0f, 0xFF, 0);

	// Bind required state
	cmdbuf.bindRasterizerState(rasterizerState);
	cmdbuf.bindColorState(colorState);
	cmdbuf.bindColorWriteState(colorWriteState);

	render_cmdlist = cmdbuf.finishList();
}

void renderGame(NVGcontext* vg, BrickGameFramework& game, float mx, float my, float width, float height, float t)
{

	switch (game.screen_orientation)
	{
	case orientation_normal:
	{
		int cell_width = 31;
		int cell_height = 31;

		int draw_grid_width = grid_width(game.game_grid);
		int draw_grid_height = grid_height(game.game_grid);

		int grid_offset_x = (1280 - (draw_grid_width * cell_width)) / 2.;
		int grid_offset_y = (720 - (draw_grid_height * cell_height)) / 2.;

		int border_size = 5;

		nvgBeginPath(vg);
		nvgRoundedRect(vg, grid_offset_x - border_size, grid_offset_y - border_size, cell_width * draw_grid_width + border_size * 2, cell_height * draw_grid_height + border_size * 2, border_size);
		nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
		nvgFill(vg);

		for (int i = 0; i < draw_grid_width; i++)
		{
			for (int j = 0; j < draw_grid_height; j++)
			{
				float x = grid_offset_x + (i)*cell_width;
				float y = grid_offset_y + (j)*cell_height;

				if (grid_get(game.game_grid, i, j))
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected");
				else
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected");
			}
		}
	}
	break;
	case (orientation_right_down):
	{
		double scale = 1.5;

		int cell_width = 31 * scale;
		int cell_height = 31 * scale;

		int draw_grid_width = grid_width(game.game_grid);
		int draw_grid_height = grid_height(game.game_grid);

		int grid_offset_x = (1280 - (draw_grid_height * cell_height)) / 2.;
		int grid_offset_y = (720 - (draw_grid_width * cell_width)) / 2.;

		int border_size = 5 * scale;

		nvgBeginPath(vg);
		nvgRoundedRect(vg, grid_offset_x - border_size, grid_offset_y - border_size, cell_width * draw_grid_height + border_size * 2, cell_height * draw_grid_width + border_size * 2, border_size);
		nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
		nvgFill(vg);

		for (int i = 0; i < draw_grid_width; i++)
		{
			for (int j = 0; j < draw_grid_height; j++)
			{
				float x = grid_offset_x + (j)*cell_height;
				float y = grid_offset_y + (i)*cell_width;

				if (grid_get(game.game_grid, draw_grid_width - i - 1, j))
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected_90");
				else
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected_90");
			}
		}
	}
	break;
	case orientation_upside_down:
	{
		int cell_width = 31;
		int cell_height = 31;

		int draw_grid_width = grid_width(game.game_grid);
		int draw_grid_height = grid_height(game.game_grid);

		int grid_offset_x = (1280 - (draw_grid_width * cell_width)) / 2.;
		int grid_offset_y = (720 - (draw_grid_height * cell_height)) / 2.;

		int border_size = 5;

		nvgBeginPath(vg);
		nvgRoundedRect(vg, grid_offset_x - border_size, grid_offset_y - border_size, cell_width * draw_grid_width + border_size * 2, cell_height * draw_grid_height + border_size * 2, border_size);
		nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
		nvgFill(vg);

		for (int i = 0; i < draw_grid_width; i++)
		{
			for (int j = 0; j < draw_grid_height; j++)
			{
				float x = grid_offset_x + (i)*cell_width;
				float y = grid_offset_y + (j)*cell_height;

				if (grid_get(game.game_grid, draw_grid_width - i - 1, draw_grid_height - j - 1))
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected_180");
				else
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected_180");
			}
		}
	}
	break;
	case (orientation_left_down):
	{
		double scale = 1.5;

		int cell_width = 31 * scale;
		int cell_height = 31 * scale;

		int draw_grid_width = grid_width(game.game_grid);
		int draw_grid_height = grid_height(game.game_grid);

		int grid_offset_x = (1280 - (draw_grid_height * cell_height)) / 2.;
		int grid_offset_y = (720 - (draw_grid_width * cell_width)) / 2.;

		int border_size = 5 * scale;

		nvgBeginPath(vg);
		nvgRoundedRect(vg, grid_offset_x - border_size, grid_offset_y - border_size, cell_width * draw_grid_height + border_size * 2, cell_height * draw_grid_width + border_size * 2, border_size);
		nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
		nvgFill(vg);

		for (int i = 0; i < draw_grid_width; i++)
		{
			for (int j = 0; j < draw_grid_height; j++)
			{
				float x = grid_offset_x + (j)*cell_height;
				float y = grid_offset_y + (i)*cell_width;

				if (grid_get(game.game_grid, i, draw_grid_height - j - 1))
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected_270");
				else
					draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected_270");
			}
		}
	}
	break;
	}
}


void BrickGameFramework::render(u64 ns)
{
	float time = ns / 1000000000.0;
	float dt = time - prevTime;
	prevTime = time;

	// Acquire a framebuffer from the swapchain (and wait for it to be available)
	int slot = queue.acquireImage(swapchain);

	// Run the command list that attaches said framebuffer to the queue
	queue.submitCommands(framebuffer_cmdlists[slot]);

	// Run the main rendering command list
	queue.submitCommands(render_cmdlist);

	updateGraph(&fps, dt);

	nvgBeginFrame(vg, FramebufferWidth, FramebufferHeight, 1.0f);
	{
		renderGame(vg, *this, 0, 0, FramebufferWidth, FramebufferHeight, time);
		renderGraph(vg, 5, 5, &fps);

		{
			nvgFontFace(vg, "sans");
			nvgFontSize(vg, 20.0f);
			nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
			nvgFillColor(vg, nvgRGBA(30, 30, 30, 255));
			nvgText(vg, 20, 100 - 30, "Welcome to this", NULL);
			nvgText(vg, 20, 120 - 30, "extremely early", NULL);
			nvgText(vg, 20, 140 - 30, "version!", NULL);
			nvgText(vg, 20, 180 - 30, "Minus: Rotate", NULL);
			nvgText(vg, 20, 200 - 30, "Y : Menu", NULL);
			nvgText(vg, 20, 220 - 30, "X : Snake", NULL);
		}
	}
	nvgEndFrame(vg);

	// Now that we are done rendering, present it to the screen
	queue.presentImage(swapchain, slot);
}

bool BrickGameFramework::onFrame(u64 ns)
{
	padUpdate(&pad);
	u64 keyboard_check_pressed = padGetButtonsDown(&pad);
	//u64 keyboard_check = padGetButtons(&pad);

	if (keyboard_check_pressed & HidNpadButton_Plus)
	{
		return false;
	}

	if (keyboard_check_pressed & HidNpadButton_Y)
	{
		next_game = 0;
	}

	if (keyboard_check_pressed & HidNpadButton_X)
	{
		next_game = 1;
	}

	if (next_game != -1 && next_game != current_game)
	{
		if (current_game != -1)
		{
			game_list.at(current_game).exit_function(*this);
		}

		current_game = next_game;
		next_game = -1;

		game_list.at(current_game).init_function(*this);
	}

	if (current_game != -1)
	{
		game_list.at(current_game).run_function(*this);
	}

	if (keyboard_check_pressed & HidNpadButton_Minus)
	{
		screen_orientation += 1;
		screen_orientation = screen_orientation % 4;
	}

	render(ns);
	return true;
}

int main(int argc, char* argv[])
{
	init_audio();
	BrickGameFramework app;
	app.run();
	exit_audio();
	return 0;
}