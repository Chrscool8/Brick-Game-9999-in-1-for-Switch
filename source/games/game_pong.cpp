#include <games/game_pong.h>
#include <controls.h>
#include <algorithm>

bool paddle_is_at(int x, int y)
{
	int paddle_width = 2;

	for (unsigned int j = 0; j < objects.size(); j++)
	{
		if (objects.at(j)->name == "obj_paddle")
			for (int i = -paddle_width; i < paddle_width; i++)
			{
				if (x == objects.at(j)->x + i && y == objects.at(j)->y)
					return true;
			}
	}
	return false;
}

subgame_pong::obj_ball::obj_ball(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	if (rand() % 2 == 0)
		hspeed = -1;
	else
		hspeed = 1;

	if (rand() % 2 == 0)
		vspeed = -1;
	else
		vspeed = 1;

	time_til_move = 60;

	name = "obj_ball";
	pause_time = 10;
}

void subgame_pong::obj_ball::step_function()
{
	if (time_til_move > 0)
	{
		time_til_move -= 1;
	}
	else
	{
		time_til_move = pause_time * fast_forwarder_half();

		if (x + hspeed < 0)
		{
			x = 0;
			hspeed = abs(hspeed);
		}
		else
			if (x + hspeed >= grid_width(game.game_grid) - 1)
			{
				x = grid_width(game.game_grid);
				hspeed = -abs(hspeed);
			}

		if (paddle_is_at(x + hspeed, y + vspeed))
			vspeed = -vspeed;

		x += hspeed;
		y += vspeed;
	}

	if (y > grid_height(game.game_grid) + 2)
	{
		game.incrementScore(-1);
		instance_destroy();
		objects.push_back(std::make_unique<obj_ball>(game, grid_width(game.game_grid) / 2, grid_height(game.game_grid) / 2));
	}

	if (y < -2)
	{
		game.incrementScore(1);
		instance_destroy();
		objects.push_back(std::make_unique<obj_ball>(game, grid_width(game.game_grid) / 2, grid_height(game.game_grid) / 2));
	}
}

void subgame_pong::obj_ball::draw_function()
{
	grid_set(game.game_grid, x, y, true);
}

void subgame_pong::obj_ball::destroy_function()
{

}

subgame_pong::subgame_pong(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Pong";
}

void subgame_pong::subgame_init()
{
	parent.setScore(0);
	objects.push_back(std::make_unique<obj_ball>(parent, grid_width(parent.game_grid) / 2, grid_height(parent.game_grid) / 2));
	objects.push_back(std::make_unique<obj_paddle>(parent, grid_width(parent.game_grid) / 2, grid_height(parent.game_grid) - 1, false));
	objects.push_back(std::make_unique<obj_paddle>(parent, grid_width(parent.game_grid) / 2, 0, true));
}

void subgame_pong::subgame_step()
{

}

void subgame_pong::subgame_draw()
{

}

void subgame_pong::subgame_exit()
{

}

void subgame_pong::subgame_demo()
{

}

std::string subgame_pong::subgame_controls_text()
{
	return "D-Pad: Move\nZL/ZR: Fast";
}


subgame_pong::obj_paddle::obj_paddle(BrickGameFramework& game, int _x, int _y, bool _ai) : game_object(game, _x, _y)
{
	paddle_width = 2;
	name = "obj_paddle";
	ai = _ai;
	pause_time = 5;
	time_til_move = 0;
}

void subgame_pong::obj_paddle::step_function()
{
	if (!ai)
	{
		if (keyboard_check_pressed_left(game) || keyboard_check_pressed_right(game))
			time_til_move = 0;

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

		if (keyboard_check_left(game) || keyboard_check_right(game))
		{
			if (time_til_move <= 0)
				time_til_move = pause_time * fast_forwarder_half();
			else
				time_til_move -= 1;
		}
	}
	else
	{
		int ball_x = -1;
		int ball_y = -1;
		for (unsigned int j = 0; j < objects.size(); j++)
		{
			if (objects.at(j)->name == "obj_ball")
			{
				ball_x = objects.at(j)->x;
				ball_y = objects.at(j)->y;
			}
		}

		bool left = (x > ball_x);
		bool right = (x < ball_x);

		if ((ball_y <= grid_height(game.game_grid) / 2 + 2) && (rand() % 5 != 0))
		{
			if (left)
			{
				if (time_til_move <= 0)
				{
					x -= 1;
				}
			}

			if (right)
			{
				if (time_til_move <= 0)
				{
					x += 1;
				}
			}

			if (left || right)
			{
				if (time_til_move <= 0)
					time_til_move = pause_time * 2 * fast_forwarder_half();
				else
					time_til_move -= 1;
			}
		}
	}
	//

	x = clamp((int)x, paddle_width, grid_width(game.game_grid) - paddle_width);
}

void subgame_pong::obj_paddle::draw_function()
{
	for (int i = -paddle_width; i < paddle_width; i++)
	{
		grid_set(game.game_grid, x + i, y, true);
	}
}

void subgame_pong::obj_paddle::destroy_function()
{

}
