#pragma once
#include <vector>
#include <map>
#include <string>
#include <grid.hpp>
using namespace std;

extern vector<vector<bool>> grid_sprite_three_x_three_square;

void place_grid_sprite(vector<vector<bool>>& grid, vector<vector<bool>>& sprite_name, int x, int y, bool additive = true);