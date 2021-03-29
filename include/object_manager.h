#pragma once
#ifndef OBJ_H
#define OBJ_H

#include <game.h>

enum enum_directions {
	direction_right,
	direction_up,
	direction_left,
	direction_down
};

struct point {
	int x;
	int y;

	point()
	{
		x = 0;
		y = 0;
	}

	point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

class game_object
{
public:
	int x;
	int y;
	static unsigned int object_index;
	unsigned int id;

	int direction;

	BrickGameFramework& game;
	vector<point> tail;

	game_object(BrickGameFramework& game_, int _x, int _y);
	vector<vector<bool>> sprite;
	virtual void create_function();
	virtual void step_function();
	virtual void draw_function();
	virtual void destroy_function();
};

extern vector<std::unique_ptr<game_object>> objects;

#endif