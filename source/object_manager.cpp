#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
#include <game.h>
using namespace std;

game_object::game_object(BrickGameFramework& _game, int _x, int _y) : game(_game), x(_x), y(_y)
{
	id = object_index;
	object_index += 1;
	printf("ID: %u\n", id);
	printf("Obj create\n");
	direction = direction_right;
	name = "object";
}

void game_object::step_function()
{
	printf("ID: %u\n", id);
	printf("Obj step\n");
};

void game_object::draw_function()
{
	printf("ID: %u\n", id);
	printf("Obj draw\n");
};

void game_object::destroy_function()
{
	printf("ID: %u\n", id);
	printf("Obj destroy\n");
};

void game_object::instance_destroy()
{
	marked_for_destruction = true;
}

obj_explosion::obj_explosion(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	animation_percent = 0;
}

void obj_explosion::step_function()
{

}

void obj_explosion::draw_function()
{
	if (animation_percent < 100)
	{
		animation_percent += 2;

		for (int i = 0; i < 360; i += 5)
		{
			double radius = 4.5;
			grid_set(game.game_grid, x + lengthdir_x(animation_percent / 100. * radius, i), y + lengthdir_y(animation_percent / 100. * radius, i), true);
			radius -= .5;
			grid_set(game.game_grid, x + lengthdir_x(animation_percent / 100. * radius, i), y + lengthdir_y(animation_percent / 100. * radius, i), true);
		}

		// Swirly
		//double radius = 3;
		//for (int i = 0; i < 360; i += 5)
		//{
		//	grid_set(game.game_grid, x + lengthdir_x(animation_percent / 100. * radius, i), y + lengthdir_y(animation_percent / 100. * radius, i), true);
		//	radius -= .5;
		//	grid_set(game.game_grid, x + lengthdir_x(animation_percent / 100. * radius, i), y + lengthdir_y(animation_percent / 100. * radius, i), true);
		//}
	}
}

void obj_explosion::destroy_function()
{

}

// Get first object of name
game_object* get_object_by_name(std::string name)
{
	for (unsigned int j = 0; j < objects.size(); j++)
	{
		if (objects.at(j)->name == name)
		{
			return objects.at(j).get();
		}
	}

	print_debug("Could not find " + name);
	return NULL;
}

unsigned int game_object::object_index = 0;

vector<std::unique_ptr<game_object>> objects;
