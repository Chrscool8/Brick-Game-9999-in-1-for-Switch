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
	game_object(BrickGameFramework& game_, int _x, int _y);
	virtual void step_function();
	virtual void draw_function();
	virtual void destroy_function();
	void instance_destroy();

	double x;
	double y;
	static unsigned int object_index;
	unsigned int id;

	int direction;

	BrickGameFramework& game;
	vector<point> tail;

	vector<vector<bool>> sprite;

	bool marked_for_destruction = false;

	std::string name;
};

class obj_explosion : public game_object
{
public:
	obj_explosion(BrickGameFramework& game, int _x, int _y);
	virtual void step_function() override;
	virtual void draw_function() override;
	virtual void destroy_function() override;
	double animation_percent;
};

game_object* get_object_by_name(std::string name);

extern vector<std::unique_ptr<game_object>> objects;

#endif