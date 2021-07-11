#pragma once
#ifndef TETRIS_H
#define TETRIS_H

#include <object_manager.h>

using namespace std;

class subgame_tetris : public subgame
{
public:
	class obj_rows : public game_object
	{
		obj_rows(BrickGameFramework& game, int starting_rows);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		vector<vector<bool>> filled_blocks;
		void shift_down();
		void check_rows();
		int lowest_occupied_line(std::vector<std::vector<bool>>& grid);
	};

	class obj_tetromino : public game_object
	{
	public:
		int angle = 0;
		int shape_index = 0;
		int color_index = 0;

		int pause_time = 5;
		int time_til_move = 0;

		bool moving = true;

		obj_tetromino(BrickGameFramework& game, int _x, int _y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;

		bool check_collision(vector<vector<bool>> shape, int _x, int _y);

		vector<vector<bool>> get_sprite(int index, int rotation);

		void move_left();
		void move_right();
	};

	subgame_tetris(BrickGameFramework& game);

	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
	virtual std::string subgame_controls_text() override;
};

#endif