#pragma once
#include "nanovg.h"
#include <demo.h>
#include <vector>
#include <string>

void renderTester(NVGcontext* vg, std::vector<std::vector<bool>> &game_grid,float mx, float my, float width, float height, float t);
void load_sprite(NVGcontext* vg, std::string sprite_name, std::string sprite_path);