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

	grid_clear(parent.game_grid);
}

void subgame_menu::subgame_run()
{
	//printf("Running Menu!!\n");

	grid_clear(parent.game_grid);

	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	bool on = ((ms.count() % 1000) < 500);

	int w = grid_width(parent.game_grid);
	int h = grid_height(parent.game_grid);

	for (int i = 0; i < w; i++)
	{
		bool my_on = (on + (i % 2)) % 2;
		grid_set(parent.game_grid, i, 0, my_on);
		grid_set(parent.game_grid, i, h - 1, !my_on);
	}

	for (int i = 0; i < h; i++)
	{
		bool my_on = (on + (i % 2)) % 2;
		grid_set(parent.game_grid, 0, i, my_on);
		if (grid_width(parent.game_grid) % 2 == 0)
			my_on = !my_on;
		grid_set(parent.game_grid, w - 1, i, my_on);
	}
}

void subgame_menu::subgame_draw()
{
}

void subgame_menu::subgame_exit()
{
	printf("Exiting Menu!!\n");
}