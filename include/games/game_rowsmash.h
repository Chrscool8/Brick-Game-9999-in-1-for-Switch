#pragma once
#ifndef ROWSMASH_H
#define ROWSMASH_H

#include <games/subgame.h>
#include <object_manager.h>
using namespace std;

class subgame_rowsmash : public subgame
{
public:
	class obj_rows : public game_object
	{
	public:
		obj_rows(BrickGameFramework& game, int starting_rows);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		vector<vector<bool>> filled_blocks;
		void shift_down();
		void check_rows();
		void lose();
		int lowest_occupied_line(std::vector<std::vector<bool>>& grid);
		void generate_row(std::vector<std::vector<bool>>& grid, int row_num);
		int pause_time;
		int time_til_move;
	};

	class obj_player : public game_object
	{
	public:
		obj_player(BrickGameFramework& game, int x, int y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;

		int pause_time = 5;
		int time_til_move = 0;
		int shoot_delay = 30;
		int time_til_shoot = 0;
	};

	class obj_bullet : public game_object
	{
	public:
		obj_bullet(BrickGameFramework& game, int x, int y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;

		int pause_time = 5;
		int time_til_move = 0;
	};

	subgame_rowsmash(BrickGameFramework& parent);

	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
	virtual std::string subgame_controls_text() override;
};

#endif
