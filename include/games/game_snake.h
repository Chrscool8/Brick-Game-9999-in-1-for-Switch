#pragma once
#ifndef SNAKE_H
#define SNAKE_H

#include <object_manager.h>

using namespace std;

class subgame_snake : public subgame
{
public:
	class obj_snake : public game_object
	{
	public:
		obj_snake(BrickGameFramework& game, int _x, int _y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		bool snake_is_at(int x, int y);
		point target;
		bool alive;
		int move_counter;
		int snake_length;
		void die();
		unsigned last_direction;
		int time_til_move;
	};
	subgame_snake(BrickGameFramework& game);

	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
	virtual std::string subgame_controls_text() override;
};


#endif