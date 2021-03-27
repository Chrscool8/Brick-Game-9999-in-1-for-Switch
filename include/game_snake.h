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
	int move_counter;
	unsigned int snake_length;
	obj_snake(BrickGame& game, int _x, int _y);
	virtual void create_function() override;
	virtual void step_function() override;
	virtual void draw_function() override;
	virtual void destroy_function() override;
};

#endif