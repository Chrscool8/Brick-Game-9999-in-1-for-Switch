#include "utils.hpp"
#include "nanovg.h"
#include <string>
#include <map>
#include "grid.hpp"

std::map<std::string, int> sprite_indicies;

void load_sprite(NVGcontext* vg, std::string sprite_name, std::string sprite_path)
{
	sprite_indicies[sprite_name] = nvgCreateImage(vg, sprite_path.c_str(), NVG_IMAGE_NEAREST);
	if (sprite_indicies[sprite_name] == 0)
		printf(("Problem loading " + sprite_name + "\n").c_str());
	else
		printf(("Loaded " + sprite_name + " to index " + std::to_string(sprite_indicies[sprite_name]) + "\n").c_str());
}

bool draw_sprite(NVGcontext* vg, float x, float y, float width, float height, std::string sprite_name)
{
	if (sprite_indicies.count(sprite_name) == 0)
	{
		printf(("Trying to draw unloaded sprite, " + sprite_name + "\n").c_str());
		return false;
	}
	else
	{
		//printf(("Trying to draw loaded sprite, " + sprite_name + "\n").c_str());
		nvgSave(vg);
		nvgScissor(vg, x, y, width, height);
		nvgTranslate(vg, x, y);

		NVGpaint  imgPaint = nvgImagePattern(vg, 0, 0, width, height, 0.0f / 180.0f * NVG_PI, sprite_indicies[sprite_name], 1.0f);
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, width, height);
		nvgFillPaint(vg, imgPaint);
		nvgFill(vg);
		nvgRestore(vg);
		return true;
	}
}

void renderTester(NVGcontext* vg, std::vector<std::vector<bool>>& game_grid, float mx, float my, float width, float height, float t)
{
	const int cell_width = 31;
	const int cell_height = 31;

	const int grid_offset_x = 260;
	const int grid_offset_y = 60;

	const int draw_grid_width = grid_width(game_grid);
	const int draw_grid_height = grid_height(game_grid);

	for (int i = 0; i < draw_grid_width; i++)
	{
		for (int j = 0; j < draw_grid_height; j++)
		{
			float x = grid_offset_x + (i)*cell_width;
			float y = grid_offset_y + (j)*cell_height;

			if (grid_get(game_grid, i, j))
				draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_selected");
			else
				draw_sprite(vg, x, y, cell_width, cell_height, "spr_cell_unselected");
		}
	}
}
