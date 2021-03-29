#pragma once
#ifndef SUBGAME_H
#define SUBGAME_H

class BrickGameFramework;

class subgame
{
public:
	BrickGameFramework& parent;

	virtual void subgame_init();
	virtual void subgame_run();
	virtual void subgame_draw();
	virtual void subgame_exit();

	subgame(BrickGameFramework& game_);
};
#endif // SUBGAME