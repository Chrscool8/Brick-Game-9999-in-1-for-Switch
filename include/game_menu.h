#pragma once
#ifndef MENU_H
#define MENU_H

#include <vector>
#include <subgame.h>
#include <object_manager.h>
using namespace std;

class subgame_menu : public subgame
{
public:

	class obj_border : public game_object
	{
	public:
		obj_border(BrickGameFramework& game);
		virtual void create_function() override;
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
	};

	class obj_welcome_text : public game_object
	{
	public:
		obj_welcome_text(BrickGameFramework& game);
		virtual void create_function() override;
		virtual void step_function() override;
		virtual void draw_function() override;
		virtual void destroy_function() override;
	};

	subgame_menu(BrickGameFramework& parent);

	virtual void subgame_init() override;
	virtual void subgame_run() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
};

#endif