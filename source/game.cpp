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
#include "audio.h"
#include <settings.h>
#include <utils/scores.h>
#include <utils/sprites.h>

#include <games/game_menu.h>
#include <games/game_snake.h>
#include <games/game_race.h>
#include <games/game_pong.h>
#include <games/game_rowfill.h>
#include <games/game_rowsmash.h>
#include <games/game_HiOrLo.h>
#include <games/game_tetris.h>

static int nxlink_sock = -1;

using namespace std;

vector<std::unique_ptr<subgame>> game_list;

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
	printf("cmdmem size: %u\n", cmdmem.getSize());
	// Create the framebuffer resources
	createFramebufferResources();

	this->renderer.emplace(FramebufferWidth, FramebufferHeight, this->device, this->queue, *this->pool_images, *this->pool_code, *this->pool_data);
	this->vg = nvgCreateDk(&*this->renderer, NVG_DEBUG);

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	// Load Resources
	load_sprite(vg, "spr_cell_selected", "romfs:/images/cell_selected.png");
	load_sprite(vg, "spr_cell_unselected", "romfs:/images/cell_unselected.png");

	load_sprite(vg, "spr_page_blip_selected", "romfs:/images/page_blip_selected.png");
	load_sprite(vg, "spr_page_blip_unselected", "romfs:/images/page_blip_unselected.png");

	for (int i = 0; i < 16; i++)
	{
		std::string num = std::to_string(i);
		if (i < 10)
			num = "0" + num;
		load_sprite(vg, "spr_cells_" + num, "romfs:/images/cells_" + num + ".png");
	}

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
	int fontSegment = nvgCreateFont(vg, "seg", "romfs:/fonts/DSEG7Classic-Bold.ttf");
	if (fontSegment == -1) {
		printf("Could not add font segment.\n");
	}
	int fontMinecraft = nvgCreateFont(vg, "minecraft", "romfs:/fonts/Minecraft.ttf");
	if (fontMinecraft == -1) {
		printf("Could not add font minecraft.\n");
	}
	int fontKongtext = nvgCreateFont(vg, "kongtext", "romfs:/fonts/kongtext-regular.ttf");
	if (fontKongtext == -1) {
		printf("Could not add font Kongtext.\n");
	}
	int vcrtext = nvgCreateFont(vg, "vcrtext", "romfs:/fonts/VCR_OSD_MONO_1.ttf");
	if (vcrtext == -1) {
		printf("Could not add font vcrtext.\n");
	}

	nvgAddFallbackFontId(vg, fontNormal, fontEmoji);
	nvgAddFallbackFontId(vg, fontBold, fontEmoji);

	printf("Done loading\n");

	//

	current_game = -1;
	next_game = 0;
	screen_orientation = orientation_normal;

	transition_stage = -1;
	transition_percent = 0;

	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);

	game_grid = grid_create(10, 20);

	game_list.push_back(std::make_unique<subgame_menu>(*this));
	game_list.push_back(std::make_unique<subgame_tetris>(*this));
	game_list.push_back(std::make_unique<subgame_snake>(*this));
	game_list.push_back(std::make_unique<subgame_race>(*this));
	game_list.push_back(std::make_unique<subgame_pong>(*this));
	game_list.push_back(std::make_unique<subgame_rowfill>(*this));
	game_list.push_back(std::make_unique<subgame_rowsmash>(*this));
	game_list.push_back(std::make_unique<subgame_HiOrLo>(*this));
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
	nvgSave(vg);

	float angle = 0;
	float scale = 1;

	switch (game.screen_orientation)
	{
	case orientation_normal:
		angle = 0;
		scale = 1;
		break;
	case orientation_left_down:
		angle = nvgDegToRad(90);
		scale = 1.5;
		break;
	case orientation_upside_down:
		angle = nvgDegToRad(180);
		scale = 1;
		break;
	case orientation_right_down:
		angle = nvgDegToRad(270);
		scale = 1.5;
		break;
	}

	nvgTranslate(vg, 1280 / 2, 720 / 2);
	nvgScale(vg, scale, scale);
	nvgRotate(vg, angle);

	int cell_width = 31;
	int cell_height = 31;

	int draw_grid_width = grid_width(game.game_grid);
	int draw_grid_height = grid_height(game.game_grid);

	int grid_offset_x = (-(draw_grid_width * cell_width)) / 2.;
	int grid_offset_y = (-(draw_grid_height * cell_height)) / 2.;

	int border_size = 5;

	nvgBeginPath(vg);
	nvgRoundedRect(vg, grid_offset_x - border_size, grid_offset_y - border_size, cell_width * draw_grid_width + border_size * 2, cell_height * draw_grid_height + border_size * 2, border_size);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, grid_offset_x, grid_offset_y, cell_width * draw_grid_width, cell_height * draw_grid_height);
	nvgFillColor(vg, nvgRGBA(109, 120, 92, 255));
	nvgFill(vg);

	for (int i = 0; i < draw_grid_width; i += 2)
	{
		for (int j = 0; j < draw_grid_height; j += 2)
		{
			float x = grid_offset_x + (i)*cell_width;
			float y = grid_offset_y + (j)*cell_height;

			bool ul = grid_get(game.game_grid, i, j);
			bool ur = grid_get(game.game_grid, i + 1, j);
			bool bl = grid_get(game.game_grid, i, j + 1);
			bool br = grid_get(game.game_grid, i + 1, j + 1);

			unsigned int pos = 1 * ul + 2 * ur + 4 * bl + 8 * br;

			std::string num = std::to_string(pos);
			if (pos < 10)
				num = "0" + num;

			if (i + 2 <= draw_grid_width && j + 2 <= draw_grid_height)
			{
				draw_sprite(vg, x, y, cell_width * 2, cell_height * 2, "spr_cells_" + num);
			}
			else
			{
				if (i < draw_grid_width && j < draw_grid_height)
				{
					if (grid_get(game.game_grid, i, j))
						draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected");
				}

				if (i + 1 < draw_grid_width && j < draw_grid_height)
				{
					if (grid_get(game.game_grid, i + 1, j))
						draw_sprite(vg, x + cell_width, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(vg, x + cell_width, y, cell_width, cell_height, "spr_cell_unselected");
				}

				if (i < draw_grid_width && j + 1 < draw_grid_height)
				{
					if (grid_get(game.game_grid, i, j + 1))
						draw_sprite(vg, x, y + cell_height, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(vg, x, y + cell_height, cell_width, cell_height, "spr_cell_unselected");
				}
			}

			//if (grid_get(game.game_grid, i, j))
			//	draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
			//else
			//	draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected");
		}
	}

	nvgRestore(vg);
}

void draw_digital_display(NVGcontext* vg, std::string display_string, int x, int y, std::string title, int angle = 0, unsigned int length = 8)
{
	nvgSave(vg);
	nvgTranslate(vg, x, y);
	nvgRotate(vg, nvgDegToRad(angle));

	nvgFontFace(vg, "kongtext");
	nvgFontSize(vg, 24);
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
	nvgText(vg, 3, 0, title.c_str(), NULL);

	nvgFontFace(vg, "seg");
	nvgFontSize(vg, 40);
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

	std::string temp_score = display_string;

	while (temp_score.size() < length)
		temp_score = " " + temp_score;

	for (unsigned int i = 0; i < length; i++)
	{
		nvgFillColor(vg, nvgRGBA(97, 112, 91, 255));
		nvgText(vg, (i * 30), 28, "8", NULL);
		nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
		std::string charat(1, temp_score.at(i));
		nvgText(vg, (i * 30), 28, charat.c_str(), NULL);
	}

	nvgRestore(vg);
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

		//if (show_ui)
		//{
		//	if (screen_orientation == orientation_normal)
		//	{
		//		renderGraph(vg, 5, 5, &fps);

		//		const float size = 25.f;
		//		nvgFontFace(vg, "minecraft");
		//		nvgFontSize(vg, size);
		//		nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
		//		nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
		//		nvgText(vg, 20, 70, "Welcome to this", NULL);
		//		nvgText(vg, 20, 70 + size * 1, "extremely early", NULL);
		//		nvgText(vg, 20, 70 + size * 2, "version!", NULL);
		//		nvgText(vg, 20, 70 + size * 4, "Minus: Rotate", NULL);
		//		nvgText(vg, 20, 70 + size * 5, "Y : Menu", NULL);
		//		nvgText(vg, 20, 70 + size * 6, "X : Snake", NULL);
		//		nvgText(vg, 20, 70 + size * 7, "A : Race", NULL);
		//		nvgText(vg, 20, 70 + size * 8, "B : Pong", NULL);
		//		nvgText(vg, 20, 70 + size * 10, "ZL/ZR : Fast Forward", NULL);
		//	}
		//}

		int wid = 8 * 30;
		if (screen_orientation == orientation_normal)
		{
			nvgSave(vg);
			nvgTranslate(vg, 150, 80);
			nvgFontFace(vg, "vcrtext");
			nvgFontSize(vg, 72);
			nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
			nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
			nvgText(vg, 3, 0, current_game_name.c_str(), NULL);
			nvgRestore(vg);

			draw_digital_display(vg, score_display, 865, 70, "Score");
			draw_digital_display(vg, highscore_display, 865, 165, "High Score");

			if (current_game != -1)
			{
				std::string controls_text = game_list.at(current_game)->subgame_controls_text();

				std::string global_controls;
				if (!controls_text.empty())
					global_controls += "\n\n";
				global_controls += "L: Toggle Music\n";
				global_controls += "R: Toggle Sounds\n";
				global_controls += "+: Exit\n";

				controls_text = "Controls:\n\n" + controls_text;
				controls_text += global_controls;

				if (!controls_text.empty())
				{
					nvgSave(vg);
					nvgTranslate(vg, 865, 475);
					nvgFontFace(vg, "kongtext");
					nvgFontSize(vg, 16);
					nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
					nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
					nvgTextBox(vg, 3, 0, 1000, controls_text.c_str(), NULL);
					nvgRestore(vg);
				}
			}
		}
		else if (screen_orientation == orientation_left_down)
		{
			draw_digital_display(vg, score_display, 1235, 720 / 2 - wid - 30, "Score", 90);
			draw_digital_display(vg, highscore_display, 1235, 720 / 2 + 30, "High Score", 90);
		}
	}
	nvgEndFrame(vg);

	// Now that we are done rendering, present it to the screen
	queue.presentImage(swapchain, slot);
}

void transition(std::vector<std::vector<bool>>& grid, double percent)
{
	//printf("%f percent\n", percent);
	if (percent > 0 && percent <= 100)
	{
		for (int y = 0; y < (double)grid_height(grid) * ((double)percent / 100.); y++)
			for (int x = 0; x < grid_width(grid); x++)
			{
				grid_set(grid, x, y, true);
			}
	}
	else if (percent > 100 && percent <= 200)
	{
		for (int y = (double)grid_height(grid) * ((double)(percent - 100) / 100.); y < grid_height(grid); y++)
			for (int x = 0; x < grid_width(grid); x++)
			{
				grid_set(grid, x, y, true);
			}
	}
}

void BrickGameFramework::SwitchToGame(int i)
{
	next_game = i;

	if (transition_percent == 0)
	{
		transition_stage = 0;
		transition_percent = 0;
	}
}

double fast_forwarder_half()
{
	return (1 - (.75 * fast_forward));
}

bool BrickGameFramework::onFrame(u64 ns)
{
	padUpdate(&pad);
	u64 keyboard_check_pressed = padGetButtonsDown(&pad);
	u64 keyboard_check = padGetButtons(&pad);

	fast_forward = ((keyboard_check & HidNpadButton_ZL) || (keyboard_check & HidNpadButton_ZR));

	if (keyboard_check_pressed & HidNpadButton_Plus)
	{
		if (current_game == 0)
			return false;
		else
			SwitchToGame(0);
	}

	if (keyboard_check_pressed & HidNpadButton_L)
	{
		if (settings_get_value_true("temp_prefs", "music_bool"))
		{
			Mix_VolumeMusic(0);
			settings_set_value("temp_prefs", "music_bool", "false");
		}
		else
		{
			Mix_VolumeMusic(128);
			settings_set_value("temp_prefs", "music_bool", "true");
		}
	}

	if (keyboard_check_pressed & HidNpadButton_R)
	{
		if (settings_get_value_true("temp_prefs", "sound_bool"))
		{
			for (auto const& [key, val] : audio_files)
				Mix_VolumeChunk(val, 0);
			settings_set_value("temp_prefs", "sound_bool", "false");
		}
		else
		{
			for (auto const& [key, val] : audio_files)
				Mix_VolumeChunk(val, 32);
			settings_set_value("temp_prefs", "sound_bool", "true");
		}
	}

	//if (keyboard_check_pressed & HidNpadButton_Y)
	//{
	//	SwitchToGame(0);
	//}

	//if (keyboard_check_pressed & HidNpadButton_X)
	//{
	//	SwitchToGame(1);
	//}

	//if (keyboard_check_pressed & HidNpadButton_A)
	//{
	//	SwitchToGame(2);
	//}

	//if (keyboard_check_pressed & HidNpadButton_B)
	//{
	//	SwitchToGame(3);
	//}

	//if (keyboard_check_pressed & HidNpadButton_R)
	//{
	//	if (target_grid_width != 20)
	//		target_grid_width = 20;
	//	else
	//		target_grid_width = 10;
	//}

	if (grid_width(game_grid) < target_grid_width)
	{
		game_grid = grid_create(grid_width(game_grid) + 1, grid_height(game_grid));
	}
	else if (grid_width(game_grid) > target_grid_width)
	{
		game_grid = grid_create(grid_width(game_grid) - 1, grid_height(game_grid));
	}

	if (grid_height(game_grid) < target_grid_height)
	{
		game_grid = grid_create(grid_width(game_grid), grid_height(game_grid) + 1);
	}
	else if (grid_height(game_grid) > target_grid_height)
	{
		game_grid = grid_create(grid_width(game_grid), grid_height(game_grid) - 1);
	}

	grid_clear(game_grid);

	if (current_game != -1)
	{
		if (transition_stage == -1)
		{
			if (running)
			{
				for (unsigned int i = 0; i < objects.size(); i++)
					objects.at(i)->step_function();

				game_list.at(current_game)->subgame_step();
			}
		}

		//
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			if (objects.at(i)->marked_for_destruction)
			{
				objects.at(i)->destroy_function();
				objects.erase(objects.begin() + i);
				i--;
			}
		}
		//

		for (unsigned int i = 0; i < objects.size(); i++)
			objects.at(i)->draw_function();

		game_list.at(current_game)->subgame_draw();
	}

	if ((next_game != -1 && next_game != current_game) && transition_stage == -1)
	{
		transition_stage = 0;
		transition_percent = 0;
	}

	if (transition_stage == 0)
	{
		if (transition_percent < 100)
		{
			transition_percent += 1.5;
		}
		else
		{
			transition_stage = 1;
			if (current_game != -1)
			{
				objects.clear();
				game_list.at(current_game)->subgame_exit();
			}

			current_game = next_game;
			next_game = -1;
			running = true;

			current_game_name = game_list.at(current_game)->name;
			setScore(0);
			highscore_display = scores_get_score_value(current_game_name, "highscore");
			try
			{
				highscore = stod(highscore_display);
			}
			catch (const std::invalid_argument& ia)
			{
				highscore = 0;
			}
			game_list.at(current_game)->subgame_init();
		}

		transition(game_grid, transition_percent);
	}
	else if (transition_stage == 1)
	{
		if (transition_percent < 200)
		{
			transition_percent += 1.5;
		}
		else
		{
			transition_percent = 0;
			transition_stage = -1;
		}

		transition(game_grid, transition_percent);
	}

	if (keyboard_check_pressed & HidNpadButton_Minus)
	{
		screen_orientation += 1;
		screen_orientation = screen_orientation % 4;
		printf("orientation: %i\n", screen_orientation);
	}

	game_time_in_frames += 1;

	render(ns);
	return true;
}

void BrickGameFramework::setScoreDisplay(std::string score)
{
	score_display = score;
}

void BrickGameFramework::setHighScoreDisplay(std::string score)
{
	highscore_display = score;
}

void BrickGameFramework::setScore(int _score)
{
	int hs = highscore;

	if (_score > highscore)
	{
		hs = _score;
		highscore = _score;
		setHighScore(std::to_string(hs));
	}

	score = _score;
	setScore(std::to_string(_score));
}

void BrickGameFramework::setScore(double _score)
{
	if (_score > highscore)
	{
		highscore = _score;
		setHighScore(std::to_string(highscore));
	}

	score = _score;
	setScore(std::to_string(_score));
}

void BrickGameFramework::setScore(std::string score)
{
	score_display = score;
	scores_set_score_value(current_game_name, "score", score);
}

void BrickGameFramework::incrementScore(int amount)
{
	setScore((int)score + amount);
}

void BrickGameFramework::setHighScore(std::string score)
{
	highscore_display = score;
	scores_set_score_value(current_game_name, "highscore", score);
}

int main(int argc, char* argv[])
{
	read_settings();
	init_settings();

	init_audio();

	read_scores();

	fast_forward = false;
	srand(time(NULL));

	BrickGameFramework app;
	app.run();

	exit_audio();
	return 0;
}