#include <stdio.h>
#include <grid.hpp>
#include <chrono>

void game_menu_init(vector<vector<bool>>& game_grid)
{
	printf("Initting Menu!!\n");

	grid_clear(game_grid);
}

void game_menu_run(vector<vector<bool>>& game_grid)
{
	//printf("Running Menu!!\n");

	grid_clear(game_grid);

	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	bool on = ((ms.count() % 1000) < 500);

	int w = grid_width(game_grid);
	int h = grid_height(game_grid);

	for (int i = 0; i < w; i++)
	{
		bool my_on = (on + (i % 2)) % 2;
		grid_set(game_grid, i, 0, my_on);
		grid_set(game_grid, i, h - 1, !my_on);
	}

	for (int i = 0; i < h; i++)
	{
		bool my_on = (on + (i % 2)) % 2;
		grid_set(game_grid, 0, i, my_on);
		grid_set(game_grid, w - 1, i, my_on);
	}
}

void game_menu_exit(vector<vector<bool>>& game_grid)
{
	printf("Exiting Menu!!\n");
}