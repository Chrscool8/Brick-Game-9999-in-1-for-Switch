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

subgame_menu::subgame_menu(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Menu";
}

void subgame_menu::subgame_init()
{
	if (parent.debug_text)
		printf("Initting Menu!!\n");
	objects.push_back(std::make_unique<obj_border>(parent));
	objects.push_back(std::make_unique<obj_welcome_text>(parent));
	parent.setScore(" XELL0 ");
	//parent.setHighScore("");
}

void subgame_menu::subgame_step()
{
}

void subgame_menu::subgame_draw()
{
}

void subgame_menu::subgame_exit()
{
	if (parent.debug_text)
		printf("Exiting Menu!!\n");
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
		x -= (1. / 15) * (2 - fast_forwarder());
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