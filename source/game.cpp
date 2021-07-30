#include <array>
#include <optional>
#include <unistd.h>
#include "perf.hpp"
#include <utils.hpp>
#include <grid.hpp>
#include <vector>
#include <game.h>
#include <nanovg/framework/CMemPool.h>
#include <nanovg/framework/CApplication.h>
#include <switch/runtime/devices/socket.h>
#include <grid_sprites.h>
#include "audio.h"
#include <utils/scores.h>
#include <games/game_menu.h>
#include <games/game_snake.h>
#include <games/game_race.h>
#include <games/game_pong.h>
#include <games/game_rowfill.h>
#include <games/game_rowsmash.h>
#include <games/game_HiOrLo.h>
#include <games/game_tetris.h>
#include <utils/settings.h>
#include <platform/control_layer.h>
#include <platform/graphics_layer.h>

using namespace std;

vector<std::unique_ptr<subgame>> game_list;

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

BrickGameFramework::BrickGameFramework()
{
	initialize_graphics(1280, 720);

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	// Load Resources
	load_sprite("spr_cell_selected", "romfs:/images/cell_selected.png");
	load_sprite("spr_cell_unselected", "romfs:/images/cell_unselected.png");

	load_sprite("spr_page_blip_selected", "romfs:/images/page_blip_selected.png");
	load_sprite("spr_page_blip_unselected", "romfs:/images/page_blip_unselected.png");

	for (int i = 0; i < 16; i++)
	{
		std::string num = std::to_string(i);
		if (i < 10)
			num = "0" + num;
		load_sprite("spr_cells_" + num, "romfs:/images/cells_" + num + ".png");
	}

	load_fonts();

	printf("Done loading\n");

	//

	current_game = -1;
	next_game = 0;
	screen_orientation = orientation_normal;

	transition_stage = -1;
	transition_percent = 0;

	init_controllers();

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
	exit_graphics();
}

void draw_grid(vector<vector<bool>> _grid, float _x, float _y, double cell_size)
{
	push_graphics();
	float angle = 0;
	float scale = 1;
	gfx_translate(_x, _y);
	gfx_rotate(angle);
	int cell_width = cell_size * scale;
	int cell_height = cell_size * scale;
	int draw_grid_width = grid_width(_grid);
	int draw_grid_height = grid_height(_grid);
	int grid_offset_x = (-(draw_grid_width * cell_width)) / 2.;
	int grid_offset_y = (-(draw_grid_height * cell_height)) / 2.;
	int border_size = 5;

	draw_rounded_rect(grid_offset_x - border_size, grid_offset_y - border_size, cell_width * draw_grid_width + border_size * 2, cell_height * draw_grid_height + border_size * 2, border_size, 0, 0, 0, 255);
	draw_rect(grid_offset_x, grid_offset_y, cell_width * draw_grid_width, cell_height * draw_grid_height, 109, 120, 92, 255);

	for (int i = 0; i < draw_grid_width; i += 2)
	{
		for (int j = 0; j < draw_grid_height; j += 2)
		{
			float x = grid_offset_x + (i)*cell_width;
			float y = grid_offset_y + (j)*cell_height;
			bool ul = grid_get(_grid, i, j);
			bool ur = grid_get(_grid, i + 1, j);
			bool bl = grid_get(_grid, i, j + 1);
			bool br = grid_get(_grid, i + 1, j + 1);
			unsigned int pos = 1 * ul + 2 * ur + 4 * bl + 8 * br;
			std::string num = std::to_string(pos);

			if (pos < 10)
				num = "0" + num;
			if (i + 2 <= draw_grid_width && j + 2 <= draw_grid_height)
			{
				draw_sprite(x, y, cell_width * 2, cell_height * 2, "spr_cells_" + num);
			}
			else
			{
				if (i < draw_grid_width && j < draw_grid_height)
				{
					if (grid_get(_grid, i, j))
						draw_sprite(x, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(x, y, cell_width, cell_height, "spr_cell_unselected");
				}
				if (i + 1 < draw_grid_width && j < draw_grid_height)
				{
					if (grid_get(_grid, i + 1, j))
						draw_sprite(x + cell_width, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(x + cell_width, y, cell_width, cell_height, "spr_cell_unselected");
				}
				if (i < draw_grid_width && j + 1 < draw_grid_height)
				{
					if (grid_get(_grid, i, j + 1))
						draw_sprite(x, y + cell_height, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(x, y + cell_height, cell_width, cell_height, "spr_cell_unselected");
				}
			}
		}
	}
	pop_graphics();
}

void renderGame(BrickGameFramework& game, float mx, float my, float t)
{
	push_graphics();

	float angle = 0;
	float scale = 1;

	switch (game.screen_orientation)
	{
	case orientation_normal:
		angle = 0;
		scale = 1;
		break;
	case orientation_left_down:
		angle = DegToRad(90);
		scale = 1.5;
		break;
	case orientation_upside_down:
		angle = DegToRad(180);
		scale = 1;
		break;
	case orientation_right_down:
		angle = DegToRad(270);
		scale = 1.5;
		break;
	}

	gfx_translate(1280 / 2, 720 / 2);
	gfx_scale(scale, scale);
	gfx_rotate(angle);

	int cell_width = 31;
	int cell_height = 31;

	int draw_grid_width = grid_width(game.game_grid);
	int draw_grid_height = grid_height(game.game_grid);

	int grid_offset_x = (-(draw_grid_width * cell_width)) / 2.;
	int grid_offset_y = (-(draw_grid_height * cell_height)) / 2.;

	int border_size = 5;

	draw_rounded_rect(grid_offset_x - border_size, grid_offset_y - border_size, cell_width * draw_grid_width + border_size * 2, cell_height * draw_grid_height + border_size * 2, border_size, 0, 0, 0, 255);
	draw_rect(grid_offset_x, grid_offset_y, cell_width * draw_grid_width, cell_height * draw_grid_height, 109, 120, 92, 255);

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
				draw_sprite(x, y, cell_width * 2, cell_height * 2, "spr_cells_" + num);
			}
			else
			{
				if (i < draw_grid_width && j < draw_grid_height)
				{
					if (grid_get(game.game_grid, i, j))
						draw_sprite(x, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(x, y, cell_width, cell_height, "spr_cell_unselected");
				}

				if (i + 1 < draw_grid_width && j < draw_grid_height)
				{
					if (grid_get(game.game_grid, i + 1, j))
						draw_sprite(x + cell_width, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(x + cell_width, y, cell_width, cell_height, "spr_cell_unselected");
				}

				if (i < draw_grid_width && j + 1 < draw_grid_height)
				{
					if (grid_get(game.game_grid, i, j + 1))
						draw_sprite(x, y + cell_height, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
					else
						draw_sprite(x, y + cell_height, cell_width, cell_height, "spr_cell_unselected");
				}
			}

			//if (grid_get(game.game_grid, i, j))
			//	draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected"); // 11% opacity
			//else
			//	draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected");
		}
	}

	pop_graphics();
}

void draw_digital_display(std::string display_string, int x, int y, std::string title, int angle = 0, unsigned int length = 8)
{
	push_graphics();
	gfx_translate(x, y);
	gfx_rotate(nvgDegToRad(angle));

	set_font("kongtext");
	set_font_size(24);
	set_text_align(TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP);
	draw_set_fill_color(0, 0, 0, 255);
	draw_text(3, 0, title.c_str());

	set_font("seg");
	set_font_size(40);
	set_text_align(TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP);

	std::string temp_score = display_string;

	while (temp_score.size() < length)
		temp_score = " " + temp_score;

	for (unsigned int i = 0; i < length; i++)
	{
		draw_set_fill_color(97, 112, 91, 255);
		draw_text((i * 30), 28, "8");
		draw_set_fill_color(0, 0, 0, 255);
		std::string charat(1, temp_score.at(i));
		draw_text((i * 30), 28, charat.c_str());
	}

	pop_graphics();
}

void BrickGameFramework::render(u64 ns)
{
	float time = ns / 1000000000.0;
	float dt = time - prevTime;
	prevTime = time;
	updateGraph(&fps, dt);

	gfx_start_frame();
	{
		renderGame(*this, 0, 0, time);

		int wid = 8 * 30;
		if (screen_orientation == orientation_normal)
		{
			push_graphics();
			gfx_translate(150, 80);
			draw_set_font("vcrtext");
			draw_set_font_size(72);
			draw_set_font_align(TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP);
			draw_set_fill_color(0, 0, 0, 255);
			draw_text(3, 0, current_game_name.c_str());
			pop_graphics();

			draw_digital_display(score_display, 865, 70, "Score");
			draw_digital_display(highscore_display, 865, 165, "High Score");

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
					push_graphics();
					gfx_translate(865, 475);
					set_font("kongtext");
					set_font_size(16);
					draw_set_font_align(TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP);
					draw_set_fill_color(0, 0, 0, 255);
					draw_text_width(3, 0, 1000, controls_text.c_str());
					pop_graphics();
				}
			}
		}
		else if (screen_orientation == orientation_left_down)
		{
			draw_digital_display(score_display, 1235, 720 / 2 - wid - 30, "Score", 90);
			draw_digital_display(highscore_display, 1235, 720 / 2 + 30, "High Score", 90);
		}
	}

	gfx_end_frame();
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
	update_controller();

	fast_forward = (keyboard_check_ZL() || keyboard_check_ZR());

	if (keyboard_check_pressed_start())
	{
		if (current_game == 0)
			return false;
		else
			SwitchToGame(0);
	}

	if (keyboard_check_pressed_L())
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

	if (keyboard_check_pressed_R())
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

	if (keyboard_check_pressed_select())
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