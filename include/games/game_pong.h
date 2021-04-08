#pragma once
#ifndef PONG_H
#define PONG_H

#include <object_manager.h>

class subgame_pong : public subgame
{
public:
	class obj_ball : public game_object
	{
	public:
		obj_ball(BrickGameFramework& game, int _x, int _y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		int time_til_move;
		int hspeed;
		int vspeed;
	};

	class obj_paddle : public game_object
	{
	public:
		obj_paddle(BrickGameFramework& game, int _x, int _y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		bool paddle_is_at(int x, int y);
		int paddle_width;
		int time_til_move;
	};

	subgame_pong(BrickGameFramework& parent);
	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
};

#endif