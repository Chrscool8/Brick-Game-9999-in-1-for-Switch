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
	subgame_menu(BrickGameFramework& parent);

	virtual void subgame_init() override;
	virtual void subgame_run() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
};

#endif