#pragma once
#ifndef GFX_LAYER_SWITCH
#define GFX_LAYER_SWITCH

#include <string>
#include <map>

extern std::map<std::string, int> sprite_indicies;

void initialize_graphics_switch(unsigned int width, unsigned int height);
void createFramebufferResources();
void destroyFramebufferResources();
void recordStaticCommands();
void load_fonts_switch();
void exit_graphics_switch();
void push_graphics_switch();
void gfx_translate_switch(float x, float y);
void gfx_scale_switch(float x, float y);
void gfx_rotate_switch(float angle);
void pop_graphics_switch();
void set_font_switch(std::string font_name);
void set_font_size_switch(float size);
void set_text_align_switch(int alignment);
void draw_set_fill_color_switch(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_text_switch(float x, float y, std::string text);
void draw_text_width_switch(float x, float y, float line_break, std::string text);
void load_sprite_switch(std::string sprite_name, std::string sprite_path);
bool draw_sprite_switch(float x, float y, float width, float height, std::string sprite_name);
void gfx_start_frame_switch();
void gfx_end_frame_switch();
void draw_rounded_rect_switch(float x, float y, float w, float h, float radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void draw_rect_switch(float x, float y, float w, float h, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void draw_set_font_switch(std::string fontname);
void draw_set_font_size_switch(float size);
void draw_set_font_align_switch(int align);

#endif // !GFX_LAYER_SWITCH