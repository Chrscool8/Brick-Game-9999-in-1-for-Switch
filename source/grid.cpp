#include <vector>
#include <cstdio>
#include <grid.hpp>

std::vector<std::vector<bool>> grid_create(int width, int height)
{
	printf("Grid create: %i, %i\n", width, height);

	std::vector<std::vector<bool>> grid;

	for (int i = 0; i < width; i++)
	{
		std::vector<bool> sub;
		for (int j = 0; j < height; j++)
		{
			sub.push_back(false);
		}
		grid.push_back(sub);
	}

	return grid;
}

bool grid_get(std::vector<std::vector<bool>>& grid, int x, int y)
{
	if (x >= 0 && x < (int)grid.size())
	{
		std::vector<bool> sub = grid.at(x);
		if (y >= 0 && y < (int)sub.size())
		{
			return sub.at(y);
		}
	}

	return false;
}

bool grid_set(std::vector<std::vector<bool>>& grid, int x, int y, bool val, bool additive)
{
	if (!additive || (additive && val))
		if (x >= 0 && x < (int)grid.size())
		{
			std::vector<bool>& sub = grid.at(x);
			if (y >= 0 && y < (int)sub.size())
			{
				sub.at(y) = val;
				return true;
			}
		}

	return false;
}

int grid_width(std::vector<std::vector<bool>>& grid)
{
	return grid.size();
}

int grid_height(std::vector<std::vector<bool>>& grid)
{
	return grid.at(0).size();
}

void grid_clear(std::vector<std::vector<bool>>& grid)
{
	for (int i = 0; i < grid_width(grid); i++)
	{
		for (int j = 0; j < grid_height(grid); j++)
		{
			grid_set(grid, i, j, false);
		}
	}
}

bool point_in_grid(std::vector<std::vector<bool>>& grid, int x, int y)
{
	return (x >= 0 && y >= 0 && x < grid_width(grid) && y < grid_height(grid));
}