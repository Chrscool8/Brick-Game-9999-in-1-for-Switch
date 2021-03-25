#pragma once
#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
using namespace std;

void game_snake_init(vector<vector<bool>>& game_grid);
void game_snake_run(vector<vector<bool>>& game_grid);
void game_snake_exit(vector<vector<bool>>& game_grid);

class obj_snake : public game_object
{
public:
	int move_counter;
	int snake_length;
	obj_snake(vector<vector<bool>>& arr, int _x, int _y);
	virtual void create_function() override;
	virtual void step_function() override;
	virtual void draw_function() override;
	virtual void destroy_function() override;
};

#endif