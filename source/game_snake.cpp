#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
#include <game_snake.h>
#include "audio.h"
#include <controls.h>
using namespace std;

// Used to create a snake at a position on the game board
subgame_snake::obj_snake::obj_snake(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	create_function();
};

// Runs for each instance of a snake object once when it's created
void subgame_snake::obj_snake::create_function()
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

// Check if any part of a snake is at this position
bool subgame_snake::obj_snake::snake_is_at(int _x, int _y)
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

// This instance of the snake dies
void subgame_snake::obj_snake::die()
{
	alive = false;
	Mix_PlayChannel(-1, audio_files["sfx_exp_odd3"], 0);

	printf("DIED!\n");
}

// Runs for each instance of a snake object each frame
void subgame_snake::obj_snake::step_function()
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

		if (keyboard_check_right(game) && last_direction != direction_left)
			direction = direction_right;

		if (keyboard_check_up(game) && last_direction != direction_down)
			direction = direction_up;

		if (keyboard_check_left(game) && last_direction != direction_right)
			direction = direction_left;

		if (keyboard_check_down(game) && last_direction != direction_up)
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
				if (!snake_is_at(x + 1, y) && point_in_grid(game.game_grid, x + 1, y))
				{
					x += 1;
				}
				else
					die();
			}
			break;
			case direction_up:
			{
				if (!snake_is_at(x, y - 1) && point_in_grid(game.game_grid, x, y - 1))
					y -= 1;
				else
					die();
			}
			break;
			case direction_left:
			{
				if (!snake_is_at(x - 1, y) && point_in_grid(game.game_grid, x - 1, y))
					x -= 1;
				else
					die();
			}
			break;
			case direction_down:
			{
				if (!snake_is_at(x, y + 1) && point_in_grid(game.game_grid, x, y + 1))
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

// Draws each frame of the game whether transitioning or not
void subgame_snake::obj_snake::draw_function()
{
	printf("ID: %u\n", id);
	printf("Snake draw\n");
	printf("%i, %i\n", x, y);

	// Draw head
	grid_set(game.game_grid, x, y, true);

	// Draw each tail segment
	for (unsigned int i = 0; i < tail.size(); i++)
		grid_set(game.game_grid, tail.at(i).x, tail.at(i).y, true);

	// Draw the pellet
	grid_set(game.game_grid, target.x, target.y, true);
};

// To delete this instance of a snake from the game
void subgame_snake::obj_snake::destroy_function()
{
	printf("ID: %u\n", id);
	printf("Snake destroy\n");
};

// Let's the subgame know what main game it belongs to.
subgame_snake::subgame_snake(BrickGameFramework& _parent) : subgame(_parent)
{
}

// Runs once when the subgame starts (when the transition shade is fully black)
void subgame_snake::subgame_init()
{
	printf("Initting Snake!!\n");
	// Create an instance of a snake object in game 'parent' at position 5, 5.
	objects.push_back(std::make_unique<obj_snake>(parent, 5, 5));
}

// Runs every frame of the subgame unless the game is transitioning
void subgame_snake::subgame_run()
{
	printf("Running Snake!!\n");
}

// Runs every frame of the subgame whether it's transitioning or not (to draw behind the shade)
void subgame_snake::subgame_draw()
{
	printf("Drawing Snake!!\n");
}

// Clean up subgame objects here, runs once when the game is changing to a different
// game and the transition shade is fully black
void subgame_snake::subgame_exit()
{
	printf("Exiting Snake!!\n");
}
