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
#include <utils/scores.h>
#include <platform/control_layer.h>
#include <platform/graphics_layer.h>

subgame_menu::subgame_menu(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Menu";
}

void subgame_menu::subgame_init()
{
	if (game.debug_text)
		printf("Initting Menu!!\n");
	objects.push_back(std::make_unique<obj_border>(game, this));
	game.setScoreDisplay("---");

	try {
		game.setHighScoreDisplay(scores_get_score_value(game_list.at(selected_game)->name, "highscore"));
	}
	catch (exception& e)
	{
		game.setHighScoreDisplay("---");
	}
}

void subgame_menu::subgame_step()
{
	if (keyboard_check_pressed_left())
	{
		selected_game -= 1;
	}

	if (keyboard_check_pressed_right())
	{
		selected_game += 1;
	}

	if (selected_game < 0)
		selected_game += game_list.size();

	if (selected_game >= game_list.size())
	{
		selected_game -= game_list.size();
	}

	if (keyboard_check_pressed_left() || keyboard_check_pressed_right())
	{
		game.setScoreDisplay("---");
		try {
			game.setHighScoreDisplay(scores_get_score_value(game_list.at(selected_game)->name, "highscore"));
		}
		catch (exception& e)
		{
			game.setHighScoreDisplay("---");
		}
	}

	if (keyboard_check_pressed_A())
	{
		game.SwitchToGame(selected_game);
	}

	ticker += 1;
	//print_debug(to_string(selected_game));
}

void subgame_menu::subgame_draw()
{
	game_list.at(selected_game)->subgame_demo();

	push_graphics();
	gfx_translate(165, 144);

	set_font("vcrtext");
	set_font_size(48);
	set_text_align(NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	draw_set_fill_color(0, 0, 0, 255);
	draw_text(3, 0, game_list.at(selected_game)->name);
	pop_graphics();

	for (int i = 0; i < game_list.size(); i++)
	{
		int size = 15;
		int xx = (1280 / 2) - (game_list.size() * (size + 15) / 2) + (i * (size + 15)) - 10;
		if (i == selected_game)
			draw_sprite(xx, 720 - 30, size, size, "spr_page_blip_selected");
		else
			draw_sprite(xx, 720 - 30, size, size, "spr_page_blip_unselected");
	}
}

void subgame_menu::subgame_exit()
{
	if (game.debug_text)
		printf("Exiting Menu!!\n");
}

void subgame_menu::subgame_demo()
{
	int xx = -((ticker / 30) % 46) + grid_width(game.game_grid);
	int yy = 6;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_w, xx, yy); xx += 6;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_e, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_l, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_c, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_o, xx, yy); xx += 5;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_m, xx, yy); xx += 6;
	place_grid_sprite(game.game_grid, grid_sprite_alphabet_e, xx, yy);
}

std::string subgame_menu::subgame_controls_text()
{
	return "D-Pad: Change\nA:     Start";
}

//

subgame_menu::obj_border::obj_border(BrickGameFramework& game, subgame_menu* par) : game_object(game, 0, 0)
{
	parent = par;
}

void subgame_menu::obj_border::step_function()
{

}

void subgame_menu::obj_border::draw_function()
{
	int w = grid_width(game.game_grid);
	int h = grid_height(game.game_grid);

	if (parent->selected_game == 0)
	{
		using namespace std::chrono;
		milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

		bool on = ((ms.count() % 1000) < 500);

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
	}
	//else
	//{
	//	for (int i = 0; i < w; i++)
	//	{
	//		grid_set(game.game_grid, i, 0, true, true);
	//		grid_set(game.game_grid, i, h - 1, true, true);
	//	}

	//	for (int i = 0; i < h; i++)
	//	{
	//		grid_set(game.game_grid, 0, i, true, true);
	//		grid_set(game.game_grid, w - 1, i, true, true);
	//	}
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