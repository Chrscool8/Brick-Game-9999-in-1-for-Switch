#pragma once
#include "nanovg.h"

#ifdef __cplusplus
extern "C" {
#endif

struct DemoData {
    int fontNormal, fontBold, fontIcons, fontEmoji;
    int images[14];
};
typedef struct DemoData DemoData;

int loadDemoData(NVGcontext* vg, DemoData* data);
void freeDemoData(NVGcontext* vg, DemoData* data);
void renderDemo(NVGcontext* vg, float mx, float my, float width, float height, float t, int blowup, DemoData* data);
void drawThumbnails(NVGcontext* vg, float x, float y, float w, float h, const int* images, int nimages, float t);

#ifdef __cplusplus
}
#endif
