#pragma once
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <games/subgame.h>
#include <object_manager.h>
using namespace std;

class subgame_template : public subgame
{
public:
	subgame_template(BrickGameFramework& game);

	virtual void subgame_init() override;
	virtual void subgame_step() override;
	virtual void subgame_draw() override;
	virtual void subgame_exit() override;
	virtual std::string subgame_controls_text() override;
};

#endif