#pragma once
#include <vector>

using namespace std;

vector<vector<bool>> grid_create(int width, int height);
bool point_in_grid(std::vector<std::vector<bool>>& grid, int x, int y);
bool grid_set(vector<vector<bool>>& grid, int x, int y, bool val, bool additive = false);
bool grid_get(vector<vector<bool>>& grid, int x, int y);
int grid_width(vector<vector<bool>>& grid);
int grid_height(vector<vector<bool>>& grid);
void grid_clear(vector<vector<bool>>& grid);
void emplace_grid_in_grid(vector<vector<bool>>& grid, vector<vector<bool>>& grid_to_place, int x, int y, bool additive);