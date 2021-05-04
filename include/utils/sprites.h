#pragma once
#ifndef SPRITES_H
#define SPRITES_H

#include <map>
#include <string>
#include <nanovg.h>

extern std::map<std::string, int> sprite_indicies;

void load_sprite(NVGcontext* vg, std::string sprite_name, std::string sprite_path);
bool draw_sprite(NVGcontext* vg, float x, float y, float width, float height, std::string sprite_name);

#endif // !SPRITES_H