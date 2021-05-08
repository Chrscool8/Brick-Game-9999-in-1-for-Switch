#include <stdio.h>
#include <game.h>
#include <games/game_HiOrLo.h>
#include <grid_sprites.h>
#include <grid_sprites_numbers.h>
#include <controls.h>
#include <utils.hpp>

subgame_HiOrLo::subgame_HiOrLo(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "HiOrLo";
}

int subgame_HiOrLo::get_next_num()
{
	unsigned int num = 1 + (int)(rand() % 9);
	return num;
}

void subgame_HiOrLo::subgame_init()
{
	if (parent.debug_text)
		printf("Initting HiOrLo!!\n");

	current_number = 5;
	next_number = get_next_num();
}

void subgame_HiOrLo::subgame_step()
{
	if (keyboard_check_pressed_up(parent))
	{
		if (next_number > current_number)
		{
			parent.incrementScore(1);
			objects.push_back(std::make_unique<obj_check>(parent, 3, 1));
		}
		else
		{
			parent.incrementScore(-1);
			objects.push_back(std::make_unique<obj_x>(parent, 3, 1));
		}
	}

	if (keyboard_check_pressed_down(parent))
	{
		if (next_number < current_number)
		{
			parent.incrementScore(1);
			objects.push_back(std::make_unique<obj_check>(parent, 3, 14));
		}
		else
		{
			parent.incrementScore(-1);
			objects.push_back(std::make_unique<obj_x>(parent, 3, 14));
		}
	}

	if (keyboard_check_pressed_A(parent))
	{
		if (next_number == current_number)
		{
			parent.incrementScore(5);
			objects.push_back(std::make_unique<obj_check>(parent, 3, 1));
			objects.push_back(std::make_unique<obj_check>(parent, 3, 14));
		}
		else
		{
			parent.incrementScore(-5);
			objects.push_back(std::make_unique<obj_x>(parent, 3, 1));
			objects.push_back(std::make_unique<obj_x>(parent, 3, 14));
		}
	}

	if (keyboard_check_pressed_up(parent) || keyboard_check_pressed_down(parent) || keyboard_check_pressed_A(parent))
	{
		current_number = next_number;
		next_number = get_next_num();
	}
}

void subgame_HiOrLo::subgame_draw()
{
	vector<vector<bool>> spr = get_gridsprite_number(current_number);
	place_grid_sprite(parent.game_grid, spr, 4, 7);

}

void subgame_HiOrLo::subgame_exit()
{
	if (parent.debug_text)
		printf("Exiting HiOrLo!!\n");
}

std::string subgame_HiOrLo::subgame_controls_text()
{
	return "Up:   Higher (1 pt)\nDown: Lower  (1 pt)\nA:    Same   (5 pts)";
}

subgame_HiOrLo::obj_check::obj_check(BrickGameFramework& game, int x, int y) : game_object(game, x, y)
{
}

void subgame_HiOrLo::obj_check::step_function()
{
	if (time_left > 0)
		time_left -= 1;
	else
		instance_destroy();
}

void subgame_HiOrLo::obj_check::draw_function()
{
	vector<vector<bool>> grid_sprite_check
	{
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 0, 1, 0, 0 },
		{ 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 }
	};

	place_grid_sprite(game.game_grid, grid_sprite_check, x, y, false);
}

void subgame_HiOrLo::obj_check::destroy_function()
{
}

subgame_HiOrLo::obj_x::obj_x(BrickGameFramework& game, int x, int y) : game_object(game, x, y)
{
}

void subgame_HiOrLo::obj_x::step_function()
{
	if (time_left > 0)
		time_left -= 1;
	else
		instance_destroy();
}

void subgame_HiOrLo::obj_x::draw_function()
{
	vector<vector<bool>> grid_sprite_check
	{
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 0, 1, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 1, 0, 1, 0 },
		{ 1, 0, 0, 0, 1 }
	};

	place_grid_sprite(game.game_grid, grid_sprite_check, x, y, false);
}

void subgame_HiOrLo::obj_x::destroy_function()
{
}
