#include <stdio.h>
#include <game.h>
#include <games/game_rowsmash.h>
#include <grid_sprites.h>
#include <controls.h>
#include <algorithm>

subgame_rowsmash::subgame_rowsmash(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Rowsmash";
}

void subgame_rowsmash::subgame_init()
{
	objects.push_back(std::make_unique<obj_rows>(game, 5));
	objects.push_back(std::make_unique<obj_player>(game, grid_width(game.game_grid) / 2, grid_height(game.game_grid) - 1));
}

void subgame_rowsmash::subgame_step()
{
}

void subgame_rowsmash::subgame_draw()
{
}

void subgame_rowsmash::subgame_exit()
{
	if (game.debug_text)
		printf("Exiting Template!!\n");
}

std::string subgame_rowsmash::subgame_controls_text()
{
	return "D-Pad: Move\nA:     Shoot";
}

//

void subgame_rowsmash::obj_rows::lose()
{
	game.running = false;
	objects.push_back(std::make_unique<obj_explosion>(game, grid_width(filled_blocks) / 2, grid_height(filled_blocks)));
}

int subgame_rowsmash::obj_rows::lowest_occupied_line(std::vector<std::vector<bool>>& grid)
{
	for (int i = grid_height(grid); i >= 0; i--)
	{
		for (int j = 0; j < grid_width(grid); j++)
		{
			if (grid_get(grid, j, i))
			{
				return i;
			}
		}
	}
	return 0;
}

void subgame_rowsmash::obj_rows::generate_row(std::vector<std::vector<bool>>& grid, int row_num)
{
	for (int i = 0; i < grid_width(grid); i++)
	{
		grid_set(grid, i, row_num, (rand() % 2) - 1);
	}
}

subgame_rowsmash::obj_rows::obj_rows(BrickGameFramework& game, int starting_rows) : game_object(game, 0, 0)
{
	filled_blocks = grid_create(grid_width(game.game_grid), grid_height(game.game_grid));
	for (int i = 0; i < starting_rows; i++)
		generate_row(filled_blocks, i);

	time_til_move = 60;
	pause_time = 60 * 4;

	name = "obj_rows";
}

void subgame_rowsmash::obj_rows::shift_down()
{
	for (int yy = grid_height(filled_blocks); yy > 0; yy--)
	{
		for (int xx = 0; xx < grid_width(filled_blocks); xx++)
		{
			grid_set(filled_blocks, xx, yy, grid_get(filled_blocks, xx, yy - 1));
		}
	}

	generate_row(filled_blocks, 0);

	if (lowest_occupied_line(filled_blocks) > grid_height(filled_blocks) - 4)
	{
		lose();
	}
}

void subgame_rowsmash::obj_rows::check_rows()
{
	for (int iii = 0; iii < grid_height(filled_blocks); iii++)
	{
		bool any_not_seen = false;
		for (int i = 0; i < grid_width(filled_blocks); i++)
		{
			if (!grid_get(filled_blocks, i, iii))
			{
				any_not_seen = true;
				break;
			}
		}

		if (!any_not_seen)
		{
			for (int jjj = iii; jjj < grid_height(filled_blocks) - 1; jjj++)
			{
				for (int j = 0; j < grid_width(filled_blocks); j++)
				{
					grid_set(filled_blocks, j, jjj, grid_get(filled_blocks, j, jjj + 1));
				}
			}

			game.incrementScore(1);
			iii = -1;
		}
	}

	if (lowest_occupied_line(filled_blocks) > grid_height(filled_blocks) - 4)
	{
		lose();
	}
}

void subgame_rowsmash::obj_rows::step_function()
{
	if (time_til_move > 0)
	{
		time_til_move -= 1;
	}
	else
	{
		time_til_move = pause_time;
		shift_down();
	}
}

void subgame_rowsmash::obj_rows::draw_function()
{
	emplace_grid_in_grid(game.game_grid, filled_blocks, 0, 0, true);
}

void subgame_rowsmash::obj_rows::destroy_function()
{

}

//

subgame_rowsmash::obj_player::obj_player(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{

}

void subgame_rowsmash::obj_player::step_function()
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
			time_til_move = pause_time;// *fast_forwarder_half();
		else
			time_til_move -= 1;
	}

	x = clamp((int)x, 0, grid_width(game.game_grid) - 1);

	////

	if (time_til_shoot > 0)
	{
		time_til_shoot -= 1;
	}

	if (keyboard_check_A(game))
	{
		if (time_til_shoot == 0)
		{
			time_til_shoot = shoot_delay;
			// shoot!
			objects.push_back(std::make_unique<obj_bullet>(game, x, y));
		}
	}
}

void subgame_rowsmash::obj_player::draw_function()
{
	grid_set(game.game_grid, x - 1, y, true);
	grid_set(game.game_grid, x + 1, y, true);
	grid_set(game.game_grid, x, y - 1, true);
	if (time_til_shoot == 0)
		grid_set(game.game_grid, x, y, true);
}

void subgame_rowsmash::obj_player::destroy_function()
{

}

/////

subgame_rowsmash::obj_bullet::obj_bullet(BrickGameFramework& game, int x, int y) : game_object(game, x, y)
{

}

void subgame_rowsmash::obj_bullet::step_function()
{
	if (time_til_move > 0)
	{
		time_til_move--;
	}
	else
	{
		game_object* obj = get_object_by_name("obj_rows");
		if (obj != NULL)
		{
			obj_rows* row_obj = static_cast<obj_rows*>(obj);

			if (y == 0)
			{
				//grid_set(row_obj->filled_blocks, x, y, true);
				instance_destroy();
				row_obj->check_rows();
				game.incrementScore(-1);
			}
			else if (grid_get(row_obj->filled_blocks, x, y))
			{
				grid_set(row_obj->filled_blocks, x, y, false);
				instance_destroy();
				row_obj->check_rows();
				game.incrementScore(1);
			}
			else if (grid_get(row_obj->filled_blocks, x, y - 1))
			{
				grid_set(row_obj->filled_blocks, x, y - 1, false);
				instance_destroy();
				row_obj->check_rows();
				game.incrementScore(1);
			}
			else if (false)
			{
				//anywhere on row
				bool any_seen = false;
				for (int i = 0; i < grid_width(row_obj->filled_blocks); i++)
				{
					if (grid_get(row_obj->filled_blocks, i, y - 1))
					{
						any_seen = true;
						break;
					}
				}

				if (any_seen)
				{
					grid_set(row_obj->filled_blocks, x, y - 1, true);
					instance_destroy();
					row_obj->check_rows();
				}
			}
		}

		y -= 1;
		time_til_move = pause_time;
	}
}

void subgame_rowsmash::obj_bullet::draw_function()
{
	grid_set(game.game_grid, x, y, true);
}

void subgame_rowsmash::obj_bullet::destroy_function()
{

}
