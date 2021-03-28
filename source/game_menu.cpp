#include <stdio.h>
#include <game.h>
#include <grid.hpp>
#include <chrono>

void game_menu_init(BrickGame& game)
{
	printf("Initting Menu!!\n");

	grid_clear(game.game_grid);
}

void game_menu_run(BrickGame& game)
{
	//printf("Running Menu!!\n");

	grid_clear(game.game_grid);

	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	bool on = ((ms.count() % 1000) < 500);

	int w = grid_width(game.game_grid);
	int h = grid_height(game.game_grid);

	for (int i = 0; i < w; i++)
	{
		bool my_on = (on + (i % 2)) % 2;
		grid_set(game.game_grid, i, 0, my_on);
		grid_set(game.game_grid, i, h - 1, !my_on);
	}

	for (int i = 0; i < h; i++)
	{
		bool my_on = (on + (i % 2)) % 2;
		grid_set(game.game_grid, 0, i, my_on);
		if (grid_width(game.game_grid) % 2 == 0)
			my_on = !my_on;
		grid_set(game.game_grid, w - 1, i, my_on);
	}
}

void game_menu_exit(BrickGame& game)
{
	printf("Exiting Menu!!\n");
}