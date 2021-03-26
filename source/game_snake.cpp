#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
#include <game_snake.h>
using namespace std;

obj_snake::obj_snake(BrickGame& game, int _x, int _y) :game_object(game, _x, _y)
{
	create_function();
};

void obj_snake::create_function()
{
	printf("ID: %u\n", id);
	printf("Snake create\n");

	move_counter = 0;
	snake_length = 3;
};

void obj_snake::step_function()
{
	printf("ID: %u\n", id);
	printf("Snake step\n");


	u64 keyboard_check_pressed = padGetButtonsDown(&game.pad);
	u64 keyboard_check = padGetButtons(&game.pad);

	if (keyboard_check & HidNpadButton_Right)
		direction = direction_right;
	if (keyboard_check & HidNpadButton_Up)
		direction = direction_up;
	if (keyboard_check & HidNpadButton_Left)
		direction = direction_left;
	if (keyboard_check & HidNpadButton_Down)
		direction = direction_down;














	if (move_counter < 25)
	{
		move_counter += 1;
	}
	else
	{
		move_counter = 0;

		tail.push_back(point(x, y));
		if (tail.size() > snake_length)
			tail.erase(tail.begin());

		switch (direction)
		{
		case direction_right:
			x += 1;
			break;
		case direction_up:
			y -= 1;
			break;
		case direction_left:
			x -= 1;
			break;
		case direction_down:
			y += 1;
			break;
		default:
			printf("No direction set\n");
		}
	}
};

void obj_snake::draw_function()
{
	printf("ID: %u\n", id);
	printf("Snake draw\n");
	printf("%i, %i\n", x, y);
	grid_set(game.game_grid, x, y, true);

	for (int i = 0; i < tail.size(); i++)
		grid_set(game.game_grid, tail.at(i).x, tail.at(i).y, true);
};

void obj_snake::destroy_function()
{
	printf("ID: %u\n", id);
	printf("Snake destroy\n");
};

void game_snake_init(BrickGame& game)
{
	printf("Initting Snake!!\n");
	objects.push_back(std::make_unique<obj_snake>(game, 5, 5));
}

void game_snake_run(BrickGame& game)
{
	printf("Running Snake!!\n");

	grid_clear(game.game_grid);

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects.at(i)->step_function();
	}

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects.at(i)->draw_function();
	}
}

void game_snake_exit(BrickGame& game)
{
	printf("Exiting Snake!!\n");

	objects.clear();
}
