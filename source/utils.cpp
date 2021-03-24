#include "utils.h"
#include "nanovg.h"

void renderTester(NVGcontext* vg, float mx, float my, float width, float height, float t, int blowup, DemoData* data)
{
	const int cell_width = 25;
	const int cell_height = 25;

	const int grid_x = 60;
	const int grid_y = 60;

	const int grid_width = 11;
	const int grid_height = 20;

	for (int i = 0; i < grid_width; i++)
	{
		for (int j = 0; j < grid_height; j++)
		{

			NVGpaint imgPaint = nvgImagePattern(vg, 0, 0, 12, 12, 0, data->images[1], 1);
			nvgBeginPath(vg);
			nvgRect(vg, grid_x + (i + .5) * cell_width, grid_y + (j + .5) * cell_height, cell_width, cell_height);
			nvgFillPaint(vg, imgPaint);
			nvgFill(vg);


			//nvgSave(vg);

			//nvgBeginPath(vg);
			//nvgCircle(vg, grid_x + (i + .5) * cell_width, grid_y + (j + .5) * cell_height, 5);
			//nvgStrokeWidth(vg, 2.0f);
			//nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
			//nvgStroke(vg);

			//nvgBeginPath(vg);
			//nvgRect(vg, grid_x + i * cell_width, grid_y + j * cell_height, cell_width, cell_height);
			//nvgStrokeWidth(vg, 2.0f);
			//nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
			//nvgStroke(vg);

			//nvgRestore(vg);

			//nvgSave(vg);
			//nvgBeginPath(vg);
			//nvgMoveTo(vg, grid_x + i * cell_width, grid_y + j * cell_height);
			//nvgLineTo(vg, grid_x + (i + 1) * cell_width, grid_y + j * cell_height);
			//nvgLineTo(vg, grid_x + (i + 1) * cell_width, grid_y + (j + 1) * cell_height);
			//nvgLineTo(vg, grid_x + i * cell_width, grid_y + (j + 1) * cell_height);
			//nvgLineTo(vg, grid_x + i * cell_width, grid_y + j * cell_height);
			//nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 255));
			//nvgStrokeWidth(vg, 5.0f);
			//nvgStroke(vg);
			//nvgRestore(vg);
		}
	}

}