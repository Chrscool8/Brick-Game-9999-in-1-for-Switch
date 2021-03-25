#pragma once
#include "nanovg.h"
#include <vector>
#include <string>

using namespace std;

void renderGame(NVGcontext* vg, vector<vector<bool>>& game_grid, float mx, float my, float width, float height, float t);
void load_sprite(NVGcontext* vg, std::string sprite_name, std::string sprite_path);