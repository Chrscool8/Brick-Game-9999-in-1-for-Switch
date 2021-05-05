#include <stdio.h>
#include <game.h>
#include <grid.hpp>
#include <chrono>
#include <games/game_menu.h>
#include <games/subgame.h>
#include <object_manager.h>
#include <grid_sprites.h>
#include <grid_sprites_alphabet.h>
#include <math.h>
#include <controls.h>
#include <utils/scores.h>
#include <utils/sprites.h>

subgame_menu::subgame_menu(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Menu";
}

void subgame_menu::subgame_init()
{
	if (parent.debug_text)
		printf("Initting Menu!!\n");
	objects.push_back(std::make_unique<obj_border>(parent));
	parent.setScoreDisplay("---");

	try {
		parent.setHighScoreDisplay(scores_get_score_value(game_list.at(selected_game)->name, "highscore"));
	}
	catch (exception& e)
	{
		parent.setHighScoreDisplay("---");
	}
}

void subgame_menu::subgame_step()
{
	if (keyboard_check_pressed_left(parent))
	{
		selected_game -= 1;
	}

	if (keyboard_check_pressed_right(parent))
	{
		selected_game += 1;
	}

	if (selected_game < 0)
		selected_game += game_list.size();

	if (selected_game >= game_list.size())
	{
		selected_game -= game_list.size();
	}

	if (keyboard_check_pressed_left(parent) || keyboard_check_pressed_right(parent))
	{
		parent.setScoreDisplay("---");
		try {
			parent.setHighScoreDisplay(scores_get_score_value(game_list.at(selected_game)->name, "highscore"));
		}
		catch (exception& e)
		{
			parent.setHighScoreDisplay("---");
		}
	}

	if (keyboard_check_pressed_A(parent))
	{
		parent.SwitchToGame(selected_game);
	}

	ticker += 1;
	//print_debug(to_string(selected_game));
}

void subgame_menu::subgame_draw()
{
	game_list.at(selected_game)->subgame_demo();

	nvgSave(parent.vg);
	nvgTranslate(parent.vg, 150 + 15, 80 + 64);
	//nvgRotate(vg, nvgDegToRad(angle));

	nvgFontFace(parent.vg, "vcrtext");
	nvgFontSize(parent.vg, 48);
	nvgTextAlign(parent.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgFillColor(parent.vg, nvgRGBA(0, 0, 0, 255));
	nvgText(parent.vg, 3, 0, game_list.at(selected_game)->name.c_str(), NULL);
	nvgRestore(parent.vg);

	for (unsigned int i = 0; i < game_list.size(); i++)
	{
		int size = 15;
		int xx = (1280 / 2) - (game_list.size() * (size + 15) / 2) + (i * (size + 15)) - 10;
		if (i == selected_game)
			draw_sprite(parent.vg, xx, 720 - 30, size, size, "spr_page_blip_selected");
		else
			draw_sprite(parent.vg, xx, 720 - 30, size, size, "spr_page_blip_unselected");
	}
}

void subgame_menu::subgame_exit()
{
	if (parent.debug_text)
		printf("Exiting Menu!!\n");
}

void subgame_menu::subgame_demo()
{
	int xx = -((ticker / 30) % 46) + grid_width(parent.game_grid);
	int yy = 6;
	place_grid_sprite(parent.game_grid, grid_sprite_alphabet_w, xx, yy); xx += 6;
	place_grid_sprite(parent.game_grid, grid_sprite_alphabet_e, xx, yy); xx += 5;
	place_grid_sprite(parent.game_grid, grid_sprite_alphabet_l, xx, yy); xx += 5;
	place_grid_sprite(parent.game_grid, grid_sprite_alphabet_c, xx, yy); xx += 5;
	place_grid_sprite(parent.game_grid, grid_sprite_alphabet_o, xx, yy); xx += 5;
	place_grid_sprite(parent.game_grid, grid_sprite_alphabet_m, xx, yy); xx += 6;
	place_grid_sprite(parent.game_grid, grid_sprite_alphabet_e, xx, yy);
}

std::string subgame_menu::subgame_controls_text()
{
	return "D-Pad: Change\nA:     Start";
}

//

subgame_menu::obj_border::obj_border(BrickGameFramework& game) : game_object(game, 0, 0)
{

}

void subgame_menu::obj_border::step_function()
{

}

void subgame_menu::obj_border::draw_function()
{
	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	bool on = ((ms.count() % 1000) < 500);

	int w = grid_width(game.game_grid);
	int h = grid_height(game.game_grid);

	for (int i = 0; i < w; i++)
	{
		bool my_on = (on + (i % 2)) % 2;
		grid_set(game.game_grid, i, 0, my_on, true);
		grid_set(game.game_grid, i, h - 1, !my_on, true);
	}

	for (int i = 0; i < h; i++)
	{
		bool my_on = (on + (i % 2)) % 2;

		grid_set(game.game_grid, 0, i, my_on, true);
		if (grid_width(game.game_grid) % 2 == 0)
			my_on = !my_on;
		grid_set(game.game_grid, w - 1, i, my_on, true);
	}

	//for (int i = 1; i < w - 1; i++)
	//{
	//	grid_set(game.game_grid, i, 1, true);
	//	grid_set(game.game_grid, i, h - 2, true);
	//}

	//for (int i = 1; i < h - 1; i++)
	//{
	//	grid_set(game.game_grid, 1, i, true);
	//	grid_set(game.game_grid, w - 2, i, true);
	//}
}

void subgame_menu::obj_border::destroy_function()
{

}

//

subgame_menu::obj_welcome_text::obj_welcome_text(BrickGameFramework& game) : game_object(game, 0, 0)
{
	// create
	x = 11.5;
	y = 5;
}

void subgame_menu::obj_welcome_text::step_function()
{
	if (x > -40)
	{
		x -= (1. / 15) * (2 - fast_forwarder_half());
	}
	else
	{
		x = grid_width(game.game_grid);
	}
}

void subgame_menu::obj_welcome_text::draw_function()
{
	int xx = floor(x);
	int yy = floor(y);
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_w, xx, yy); xx += 6;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_e, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_l, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_c, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_o, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_m, xx, yy); xx += 6;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_e, xx, yy);
}

void subgame_menu::obj_welcome_text::destroy_function()
{

}