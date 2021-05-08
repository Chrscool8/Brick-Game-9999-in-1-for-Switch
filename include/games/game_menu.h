#pragma once
#ifndef MENU_H
#define MENU_H

#include <vector>
#include <games/subgame.h>
#include <object_manager.h>
using namespace std;

class subgame_menu : public subgame
{
public:

	class obj_border : public game_object
	{
	public:
		obj_border(BrickGameFramework& game);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
	};

	class obj_welcome_text : public game_object
	{
	public:
		obj_welcome_text(BrickGameFramework& game);
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
	};

	subgame_menu(BrickGameFramework& game);

	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
	virtual void subgame_demo() override;
	virtual std::string subgame_controls_text() override;

	int selected_game = 0;
	unsigned int ticker = 0;
};

#endif