#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
#include <game_snake.h>
#include "audio.h"
using namespace std;

obj_snake::obj_snake(BrickGame& game, int _x, int _y) : game_object(game, _x, _y)
{
	create_function();
};

void obj_snake::create_function()
{
	printf("ID: %u\n", id);
	printf("Snake create\n");

	move_counter = 0;
	snake_length = 3;
	alive = true;
	target = point(rand() % grid_width(game.game_grid), rand() % grid_height(game.game_grid));
	direction = direction_right;
	last_direction = direction_right;
	time_til_move = 25;
};

bool keyboard_check_right(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Right);
}

bool keyboard_check_up(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Up);
}

bool keyboard_check_left(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Left);
}

bool keyboard_check_down(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Down);
}

bool obj_snake::snake_is_at(int _x, int _y)
{
	if (x == _x && y == _y)
	{
		return true;
	}

	for (unsigned int i = 0; i < tail.size(); i++)
	{
		point p = tail.at(i);
		if (p.x == _x && p.y == _y)
		{
			return true;
		}
	}

	return false;
}

void obj_snake::die()
{
	alive = false;
	Mix_PlayChannel(-1, audio_files["sfx_exp_odd3"], 0);

	printf("DIED!\n");
}

void obj_snake::step_function()
{
	printf("ID: %u\n", id);
	printf("Snake step\n");

	if (alive)
	{
		if (x == target.x && y == target.y)
		{
			int attempts = 10000;
			do {
				target = point(rand() % grid_width(game.game_grid), rand() % grid_height(game.game_grid));
				attempts -= 1;
			} while (attempts > 0 && snake_is_at(target.x, target.y));

			snake_length += 1;
			time_til_move = max(10, time_til_move - 2);
			Mix_PlayChannel(-1, audio_files["sfx_sounds_button6"], 0);
		}

		if (keyboard_check_right(game) && last_direction != direction_left) // !snake_is_at(x + 1, y))
			direction = direction_right;

		if (keyboard_check_up(game) && last_direction != direction_down) // !snake_is_at(x, y - 1))
			direction = direction_up;

		if (keyboard_check_left(game) && last_direction != direction_right) // !snake_is_at(x - 1, y))
			direction = direction_left;

		if (keyboard_check_down(game) && last_direction != direction_up) // !snake_is_at(x, y + 1))
			direction = direction_down;

		if (move_counter < time_til_move)
		{
			move_counter += 1;
		}
		else
		{
			Mix_PlayChannel(-1, audio_files["sfx_movement_footsteps5"], 0);
			move_counter = 0;

			tail.push_back(point(x, y));
			if (tail.size() > snake_length)
				tail.erase(tail.begin());

			switch (direction)
			{
			case direction_right:
			{
				if (!snake_is_at(x + 1, y))
				{
					x += 1;
				}
				else
					die();
			}
			break;
			case direction_up:
			{
				if (!snake_is_at(x, y - 1))
					y -= 1;
				else
					die();
			}
			break;
			case direction_left:
			{
				if (!snake_is_at(x - 1, y))
					x -= 1;
				else
					die();
			}
			break;
			case direction_down:
			{
				if (!snake_is_at(x, y + 1))
					y += 1;
				else
					die();
			}
			break;
			default:
				printf("No direction set\n");
			}

			last_direction = direction;
		}
	}
};

void obj_snake::draw_function()
{
	printf("ID: %u\n", id);
	printf("Snake draw\n");
	printf("%i, %i\n", x, y);
	grid_set(game.game_grid, x, y, true);

	for (unsigned int i = 0; i < tail.size(); i++)
		grid_set(game.game_grid, tail.at(i).x, tail.at(i).y, true);

	grid_set(game.game_grid, target.x, target.y, true);
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
