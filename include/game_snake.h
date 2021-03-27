#pragma once
#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
#include <game.h>
using namespace std;

void game_snake_init(BrickGame& game);
void game_snake_run(BrickGame& game);
void game_snake_exit(BrickGame& game);

class obj_snake : public game_object
{
public:
	point target;
	bool alive;
	int move_counter;
	unsigned int snake_length;
	obj_snake(BrickGame& game, int _x, int _y);
	virtual void create_function() override;
	bool snake_is_at(int x, int y);
	virtual void step_function() override;
	virtual void draw_function() override;
	virtual void destroy_function() override;
	void die();
	unsigned last_direction;
	int time_til_move;
};

#endif