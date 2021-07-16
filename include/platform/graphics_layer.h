#pragma once
#include <string>

#define TEXT_ALIGN_LEFT 1
#define TEXT_ALIGN_TOP 8

void initialize_graphics(unsigned int width, unsigned int height);
void load_sprite(std::string sprite_name, std::string filename);
bool draw_sprite(float x, float y, float width, float height, std::string sprite_name);
void load_fonts();
void exit_graphics();
void push_graphics();
void gfx_translate(float x, float y);
void gfx_scale(float x, float y);
void gfx_rotate(float angle);
void pop_graphics();
void set_font(std::string font_name);
void set_font_size(float size);
void set_text_align(int alignment);
void draw_set_fill_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_text(float x, float y, std::string text);
void draw_text_width(float x, float y, float line_break, std::string text);
void gfx_start_frame();
void gfx_end_frame();
void draw_rounded_rect(float x, float y, float w, float h, float radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void draw_rect(float x, float y, float w, float h, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void draw_set_font(std::string fontname);
void draw_set_font_size(float size);
void draw_set_font_align(int align);
