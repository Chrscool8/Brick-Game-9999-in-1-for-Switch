#pragma once
#ifndef HiOrLo_H
#define HiOrLo_H

#include <games/subgame.h>
#include <object_manager.h>
using namespace std;

class subgame_HiOrLo : public subgame
{
public:
	subgame_HiOrLo(BrickGameFramework& game);

	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
	virtual std::string subgame_controls_text() override;
	int get_next_num();
	unsigned int current_number;
	unsigned int next_number;

	class obj_check : public game_object
	{
	public:
		obj_check(BrickGameFramework& game, int x, int y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;

		int time_left = 60;
	};

	class obj_x : public game_object
	{
	public:
		obj_x(BrickGameFramework& game, int x, int y);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;

		int time_left = 60;
	};
};

#endif