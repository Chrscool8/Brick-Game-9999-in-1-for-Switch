#pragma once
#include <string>
#include "nanovg.h"
#include <demo.h>

void renderTester(NVGcontext* vg, float mx, float my, float width, float height, float t, int blowup, DemoData* data);
void load_sprite(NVGcontext* vg, std::string sprite_name, std::string sprite_path);