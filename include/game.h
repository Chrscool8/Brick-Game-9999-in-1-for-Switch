#pragma once
#ifndef GAME_H
#define GAME_H

#include <string>
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
#include <games/subgame.h>

class BrickGameFramework;

enum enum_orientation {
	orientation_normal,
	orientation_left_down,
	orientation_upside_down,
	orientation_right_down
};

extern vector<std::unique_ptr<subgame>> game_list;

class BrickGameFramework : public CApplication
{
private:
	int current_game;
	int next_game;
	std::string current_game_name;

	PerfGraph fps;
	float prevTime;

	double score = 0;
	double highscore = 0;

	std::string highscore_display = "";
	std::string score_display = "";

public:

	bool running;

	BrickGameFramework();
	~BrickGameFramework();

	void render(u64 ns);
	bool onFrame(u64 ns) override;

	vector<vector<bool>> game_grid;

	char screen_orientation;
	int transition_stage;
	double transition_percent;

	unsigned int game_time_in_frames = 0;

	int target_grid_width = 10;
	int target_grid_height = 20;

	bool show_ui = true;
	bool debug_text = true;

	void setScoreDisplay(std::string score);
	void setHighScoreDisplay(std::string score);
	void setScore(int score);
	void setScore(double score);
	void setScore(std::string score);
	void incrementScore(int amount);
	void setHighScore(std::string score);

	void SwitchToGame(int i);
};

static bool fast_forward = false;
double fast_forwarder_half();

void draw_grid(vector<vector<bool>> _grid, float _x, float _y, double cell_size);

void renderGame(BrickGameFramework& game, float mx, float my, float t);

#endif // !GAME_H