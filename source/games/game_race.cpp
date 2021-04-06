#include <vector>
#include <games/game_race.h>
#include <grid_sprites.h>
#include <controls.h>
#include <algorithm>

using namespace std;

vector<vector<bool>> grid_sprite_racecar
{
	{ 0, 1, 0 },
	{ 1, 1, 1 },
	{ 0, 1, 0 },
	{ 1, 0, 1 }
};

subgame_race::obj_player_car::obj_player_car(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	time_til_move = 5;
}

void subgame_race::obj_player_car::step_function()
{
	if (keyboard_check_pressed_left(game) || keyboard_check_pressed_right(game) || keyboard_check_pressed_up(game) || keyboard_check_pressed_down(game))
		time_til_move = 0;

	//

	if (keyboard_check_left(game))
	{
		if (time_til_move <= 0)
		{
			x -= 1;
		}
	}

	if (keyboard_check_right(game))
	{
		if (time_til_move <= 0)
		{
			x += 1;
		}
	}

	if (keyboard_check_up(game))
	{
		if (time_til_move <= 0)
		{
			y -= 1;
		}
	}

	if (keyboard_check_down(game))
	{
		if (time_til_move <= 0)
		{
			y += 1;
		}
	}

	if (keyboard_check_left(game) || keyboard_check_right(game) || keyboard_check_up(game) || keyboard_check_down(game))
	{
		if (time_til_move <= 0)
			time_til_move = 5;
		else
			time_til_move -= 1;
	}

	//

	x = clamp((int)x, 2, grid_width(game.game_grid) - 3);
	y = clamp((int)y, 2, grid_height(game.game_grid) - 4);
}

void subgame_race::obj_player_car::draw_function()
{
	place_grid_sprite(game.game_grid, grid_sprite_racecar, x - 1, y - 1);
}

void subgame_race::obj_player_car::destroy_function()
{
}

subgame_race::subgame_race(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Race";
	_parent.setScore(0);
}

subgame_race::obj_border::obj_border(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	ticker = 0;
}

void subgame_race::obj_border::step_function()
{
	ticker += .15;
}

void subgame_race::obj_border::draw_function()
{
	for (int i = 0; i < grid_height(game.game_grid); i++)
	{
		bool on = (((grid_height(game.game_grid) - i) + (int)ticker) % 6) >= 3;
		grid_set(game.game_grid, 0, i, on, true);
		grid_set(game.game_grid, grid_width(game.game_grid) - 1, i, on, true);
	}
}

void subgame_race::obj_border::destroy_function()
{

}

void subgame_race::subgame_init()
{
	printf("Initting Race!!\n");
	objects.push_back(std::make_unique<obj_border>(parent, 0, 0));
	objects.push_back(std::make_unique<obj_player_car>(parent, 5, 5));
	timer = 0;
	parent.setScore(0);
}

void subgame_race::subgame_step()
{
	if (timer < 3 * 60 * .5)
	{
		timer += 1;
	}
	else
	{
		timer = 0;
		const int lane_width = 3;
		int lane = rand() % ((grid_width(parent.game_grid) - 2) / lane_width);
		lane = lane * lane_width + 2 + 1;
		lane = clamp((int)lane, 2, grid_width(parent.game_grid) - 3 - 1);

		printf("lane: %i\n", lane);
		objects.push_back(std::make_unique<obj_player_ai>(parent, lane, -5));
	}
}

void subgame_race::subgame_draw()
{

}

void subgame_race::subgame_exit()
{

}

subgame_race::obj_player_ai::obj_player_ai(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
}

void subgame_race::obj_player_ai::step_function()
{
	if (y < grid_height(game.game_grid))
	{
		y += .15;
	}
	else
	{
		instance_destroy();
	}
}

void subgame_race::obj_player_ai::draw_function()
{
	place_grid_sprite(game.game_grid, grid_sprite_racecar, x - 1, y - 1);
}

void subgame_race::obj_player_ai::destroy_function()
{
	printf("Kerblam\n");
	game.incrementScore(1);
}
