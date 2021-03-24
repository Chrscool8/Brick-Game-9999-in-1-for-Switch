#pragma once
#include <array>
#include <optional>
#include <unistd.h>
#include "perf.hpp"
#include <utils.hpp>
#include <grid.hpp>
#include <vector>
#include <game.hpp>
#include "nanovg.h"
#include "nanovg_dk.h"
#include <nanovg/framework/CMemPool.h>
#include <nanovg/framework/CApplication.h>
#include <switch/runtime/devices/socket.h>
#define NumFramebuffers 2

class BrickGame : public CApplication
{
private:
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
	PadState pad;

public:
	BrickGame();
	~BrickGame();
	void createFramebufferResources();
	void destroyFramebufferResources();
	void recordStaticCommands();
	void render(u64 ns, int blowup);
	bool onFrame(u64 ns) override;

	vector<vector<bool>> game_grid;
};