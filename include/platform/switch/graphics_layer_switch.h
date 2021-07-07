#pragma once
#include <string>
#include <nanovg_dk.h>

//class graph_lib;
//extern graph_lib GL;

class graph_lib
{
public:
	uint32_t FramebufferWidth;
	uint32_t FramebufferHeight;
	unsigned StaticCmdSize;

	dk::UniqueDevice device;
	dk::UniqueQueue queue;

	std::optional<CMemPool> pool_images;
	std::optional<CMemPool> pool_code;
	std::optional<CMemPool> pool_data;

	dk::UniqueCmdBuf cmdbuf;

	static const int NumFramebuffers = 2;

	CMemPool::Handle depthBuffer_mem;
	CMemPool::Handle framebuffers_mem[NumFramebuffers];

	dk::Image depthBuffer;
	dk::Image framebuffers[NumFramebuffers];
	DkCmdList framebuffer_cmdlists[NumFramebuffers];
	dk::UniqueSwapchain swapchain;

	DkCmdList render_cmdlist;
	NVGcontext* vg;

	std::optional<nvg::DkRenderer> renderer;

	int slot;
};

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
void gfx_set_fill_color_switch(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_text_switch(float x, float y, std::string text);
extern std::map<std::string, int> sprite_indicies;
void load_sprite_switch(std::string sprite_name, std::string sprite_path);
bool draw_sprite_switch(float x, float y, float width, float height, std::string sprite_name);
void gfx_start_frame_switch();
void gfx_end_frame_switch();
