#include <vector>
#include <cstdio>

std::vector<std::vector<bool>> grid_create(int width, int height)
{
	printf("Grid create\n");

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
	std::vector<bool> sub = grid.at(x);
	return sub.at(y);
}

void grid_set(std::vector<std::vector<bool>>& grid, int x, int y, bool val)
{
	std::vector<bool>& sub = grid.at(x);
	sub.at(y) = val;
}

int grid_width(std::vector<std::vector<bool>>& grid)
{
	return grid.size();
}

int grid_height(std::vector<std::vector<bool>>& grid)
{
	return grid.at(0).size();
}