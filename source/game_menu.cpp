#include <stdio.h>
#include <game.h>
#include <grid.hpp>
#include <chrono>
#include <game_menu.h>
#include <subgame.h>
#include <object_manager.h>

subgame_menu::subgame_menu(BrickGameFramework& _parent) : subgame(_parent)
{

}

void subgame_menu::subgame_init()
{
	printf("Initting Menu!!\n");
	objects.push_back(std::make_unique<obj_border>(parent));
	parent.score = " XELL0  ";
}

void subgame_menu::subgame_run()
{
}

void subgame_menu::subgame_draw()
{
}

void subgame_menu::subgame_exit()
{
	printf("Exiting Menu!!\n");
}

subgame_menu::obj_border::obj_border(BrickGameFramework& game) : game_object(game, 0, 0)
{
}

void subgame_menu::obj_border::create_function()
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
}

void subgame_menu::obj_border::destroy_function()
{
}
