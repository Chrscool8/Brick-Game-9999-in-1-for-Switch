#pragma once
#ifndef GAME_H
#define GAME_H

#include <array>
#include <optional>
#include <unistd.h>
#include "perf.hpp"
#include <utils.hpp>
#include <grid.hpp>
#include <vector>
#include <nanovg/framework/CMemPool.h>
#include <nanovg/framework/CApplication.h>
#include <nanovg/dk_renderer.hpp>
#define NumFramebuffers 2

class BrickGame;

struct game_item {
	std::string name;
	void (*init_function)(BrickGame& game);
	void (*run_function)(BrickGame& game);
	void (*exit_function)(BrickGame& game);
};

class BrickGame : public CApplication
{
private:
	int current_game;
	int next_game;

	uint32_t FramebufferWidth;
	uint32_t FramebufferHeight;
	unsigned StaticCmdSize;

	dk::UniqueDevice device;
	dk::UniqueQueue queue;

	std::optional<CMemPool> pool_images;
	std::optional<CMemPool> pool_code;
	std::optional<CMemPool> pool_data;

	dk::UniqueCmdBuf cmdbuf;

	CMemPool::Handle depthBuffer_mem;
	CMemPool::Handle framebuffers_mem[NumFramebuffers];

	dk::Image depthBuffer;
	dk::Image framebuffers[NumFramebuffers];
	DkCmdList framebuffer_cmdlists[NumFramebuffers];
	dk::UniqueSwapchain swapchain;

	DkCmdList render_cmdlist;

	std::optional<nvg::DkRenderer> renderer;
	NVGcontext* vg;

	PerfGraph fps;
	float prevTime;

	bool portrait_mode;
	vector<game_item> game_list;

public:
	BrickGame();
	~BrickGame();
	void createFramebufferResources();
	void destroyFramebufferResources();
	void recordStaticCommands();
	void render(u64 ns);
	bool onFrame(u64 ns) override;

	vector<vector<bool>> game_grid;
	PadState pad;
};

#endif // !GAME_H