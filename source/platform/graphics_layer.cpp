#include <platform/graphics_layer.h>
#include <platform/switch/graphics_layer_switch.h>

void initialize_graphics(unsigned int width, unsigned int height)
{
	initialize_graphics_switch(width, height);
}

void load_sprite(std::string sprite_name, std::string filename)
{
	load_sprite_switch(sprite_name, filename);
}

bool draw_sprite(float x, float y, float width, float height, std::string sprite_name)
{
	return draw_sprite_switch(x, y, width, height, sprite_name);
}

void load_fonts()
{
	load_fonts_switch();
}

void exit_graphics()
{
	exit_graphics_switch();
}

void push_graphics()
{
	push_graphics_switch();
}

void gfx_translate(float x, float y)
{
	gfx_translate_switch(x, y);
}

void gfx_scale(float x, float y)
{
	gfx_scale_switch(x, y);
}

void gfx_rotate(float angle)
{
	gfx_rotate_switch(angle);
}

void pop_graphics()
{
	pop_graphics_switch();
}

void set_font(std::string font_name)
{
	set_font_switch(font_name);
}

void set_font_size(float size)
{
	set_font_size_switch(size);
}

void set_text_align(int alignment)
{
	set_text_align_switch(alignment);
}

void draw_set_fill_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	draw_set_fill_color_switch(r, g, b, a);
}

void draw_text(float x, float y, std::string text)
{
	draw_text_switch(x, y, text);
}

void draw_text_width(float x, float y, float line_break, std::string text)
{
	draw_text_width_switch(x, y, line_break, text);
}

void gfx_start_frame()
{
	gfx_start_frame_switch();
}

void gfx_end_frame()
{
	gfx_end_frame_switch();
}

void draw_rounded_rect(float x, float y, float w, float h, float radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	draw_rounded_rect_switch(x, y, w, h, radius, red, green, blue, alpha);
}

void draw_rect(float x, float y, float w, float h, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	draw_rect_switch(x, y, w, h, red, green, blue, alpha);
}

void draw_set_font(std::string fontname)
{
	draw_set_font_switch(fontname);
}

void draw_set_font_size(float size)
{
	draw_set_font_size_switch(size);
}

void draw_set_font_align(int align)
{
	draw_set_font_align_switch(align);
}
