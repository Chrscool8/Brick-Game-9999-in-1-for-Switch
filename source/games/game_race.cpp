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
	time_til_move_x = 5;
}

void subgame_race::obj_player_car::step_function()
{
	if (keyboard_check_pressed_left(game) || keyboard_check_pressed_right(game))
		time_til_move_x = 0;

	if (keyboard_check_left(game))
	{
		if (time_til_move_x <= 0)
		{
			time_til_move_x = 5;
			x -= 1;
		}
		else
		{
			time_til_move_x -= 1;
		}
	}

	if (keyboard_check_right(game))
	{
		if (time_til_move_x <= 0)
		{
			time_til_move_x = 5;
			x += 1;
		}
		else
		{
			time_til_move_x -= 1;
		}
	}

	//

	if (keyboard_check_pressed_up(game) || keyboard_check_pressed_down(game))
		time_til_move_y = 0;

	if (keyboard_check_up(game))
	{
		if (time_til_move_y <= 0)
		{
			time_til_move_y = 5;
			y -= 1;
		}
		else
		{
			time_til_move_y -= 1;
		}
	}

	if (keyboard_check_down(game))
	{
		if (time_til_move_y <= 0)
		{
			time_til_move_y = 5;
			y += 1;
		}
		else
		{
			time_til_move_y -= 1;
		}
	}

	//

	x = clamp((int)x, 2, grid_width(game.game_grid) - 3);
	y = clamp((int)y, 2, grid_height(game.game_grid) - 3);
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
}

void subgame_race::subgame_init()
{
	printf("Initting Race!!\n");
	objects.push_back(std::make_unique<obj_player_car>(parent, 5, 5));
}

void subgame_race::subgame_run()
{
}

void subgame_race::subgame_draw()
{
}

void subgame_race::subgame_exit()
{
}
