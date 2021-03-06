#pragma once
#ifndef RACE_H
#define RACE_H

#include <object_manager.h>
#include <vector>
using namespace std;

extern vector<vector<bool>> grid_sprite_racecar;

class subgame_race : public subgame
{
public:
	class obj_border : public game_object
	{
	public:
		obj_border(BrickGameFramework& game, int _x, int _y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		double ticker;
	};

	class obj_player_car : public game_object
	{
	public:
		obj_player_car(BrickGameFramework& game, int _x, int _y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		int time_til_move;
		void die();
	};

	class obj_player_ai : public game_object
	{
	public:
		obj_player_ai(BrickGameFramework& game, int _x, int _y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		int time_til_move = 0;
		int pause_time = 6;
	};

	subgame_race(BrickGameFramework& game);
	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
	virtual void subgame_demo() override;
	virtual std::string subgame_controls_text() override;
	int time_til_spawn = 180;
	int pause_time = 180;
};

#endif // !RACE_H