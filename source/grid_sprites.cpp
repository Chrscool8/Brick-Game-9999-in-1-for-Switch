#include <vector>
#include <map>
#include <string>
#include <grid.hpp>
#include <grid_sprites.h>
using namespace std;

vector<vector<bool>> grid_sprite_three_x_three_square
{
	{ 1, 1, 1 },
	{ 1, 0, 1 },
	{ 1, 1, 1 }
};

void place_grid_sprite(vector<vector<bool>>& grid, vector<vector<bool>>& sprite_name, int x, int y, bool additive)
{
	// printf("draw sprite: %i, %i\n", x, y);

	vector<vector<bool>> sprite = sprite_name;
	int sprite_width = grid_width(sprite);
	int sprite_height = grid_height(sprite);

	for (int i = 0; i < sprite_width; i++)
	{
		for (int j = 0; j < sprite_height; j++)
		{
			if (additive)
			{
				if (grid_get(sprite, i, j))
					grid_set(grid, x + j, y + i, true);
			}
			else
				grid_set(grid, x + j, y + i, grid_get(sprite, i, j));
		}
	}
}