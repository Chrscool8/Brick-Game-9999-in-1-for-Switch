#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
using namespace std;




game_object::game_object(vector<vector<bool>>& arr, int _x, int _y) : gg(arr), x(_x), y(_y)
{
	id = object_index;
	object_index += 1;
	create_function();
	direction = direction_right;
}

void game_object::create_function()
{
	printf("ID: %u\n", id);
	printf("Obj create\n");
};

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

unsigned int game_object::object_index = 0;

vector<std::unique_ptr<game_object>> objects;
