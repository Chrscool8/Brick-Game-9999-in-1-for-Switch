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
		point target;
		bool alive;
		int move_counter;
		unsigned int snake_length;
		obj_snake(BrickGameFramework& game, int _x, int _y);
		virtual void create_function() override;
		bool snake_is_at(int x, int y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
		void die();
		unsigned last_direction;
		int time_til_move;
	};
	subgame_snake(BrickGameFramework& parent);

	virtual void subgame_init() override;
	virtual void subgame_run() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
};


#endif