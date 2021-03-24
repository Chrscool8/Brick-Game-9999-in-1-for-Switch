#pragma once
#include <vector>

std::vector<std::vector<bool>> grid_create(int width, int height);
void grid_set(std::vector<std::vector<bool>>& grid, int x, int y, bool val);
bool grid_get(std::vector<std::vector<bool>> &grid, int x, int y);
int grid_width(std::vector<std::vector<bool>> &grid);
int grid_height(std::vector<std::vector<bool>> &grid);