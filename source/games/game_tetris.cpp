#include <games/game_tetris.h>
#include <games/game_race.h>
#include <grid_sprites.h>
#include <controls.h>
#include <game_tetris_shapes.h>

// Let's the subgame know what main game it belongs to.
subgame_tetris::subgame_tetris(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Tetris";
}

// Runs once when the subgame starts (when the transition shade is fully black)
void subgame_tetris::subgame_init()
{
	printf("Initting Tetris!!\n");
	// Create an instance of a snake object in game 'game' at position 5, 5.
	objects.push_back(std::make_unique<obj_tetris_rows>(game));
}

// Runs every frame of the subgame unless the game is transitioning
void subgame_tetris::subgame_step()
{
	if (phase == -1)
	{
		// Be Prepared
		phase = 0;
	}
	else if (phase == 0)
	{
		// Create Object
		objects.push_back(std::make_unique<obj_tetromino>(game, grid_width(game.game_grid) / 2 - 1, -2));
		phase = 1;
	}
	else if (phase == 1)
	{
		// Wait for no more object
		if (!object_exists("obj_tetromino"))
		{
			phase = 2;
		}
	}
	else if (phase == 2)
	{
		highlighted_rows.clear();
		// Check Rows
		game_object* rows = get_object_by_name("obj_tetris_rows");
		if (rows != NULL)
		{
			obj_tetris_rows* row_obj = static_cast<obj_tetris_rows*>(rows);
			int width = grid_width(row_obj->filled_blocks);
			int height = grid_height(row_obj->filled_blocks);
			print_debug(to_string(width) + " " + to_string(height));
			for (int i = 0; i < height; i++)
			{
				bool filled = true;
				for (int j = 0; j < width; j++)
				{
					if (!grid_get(row_obj->filled_blocks, j, i))
					{
						filled = false;
						break;
					}
				}

				if (filled)
				{
					highlighted_rows.push_back(i);
					print_debug(to_string(i));
				}
			}
		}

		phase = 3;
		ticker = 0;
	}
	else if (phase == 3)
	{
		// Animate Rows
		ticker += 1;
		game_object* rows = get_object_by_name("obj_tetris_rows");
		if (rows != NULL)
		{
			obj_tetris_rows* row_obj = static_cast<obj_tetris_rows*>(rows);
			for (unsigned int i = 0; i < highlighted_rows.size(); i++)
			{
				for (int j = 0; j < grid_width(row_obj->filled_blocks); j++)
				{
					grid_set(row_obj->filled_blocks, j, highlighted_rows.at(i), (ticker % 50) > 25);
				}
			}
		}

		if (ticker > 150 || highlighted_rows.empty())
			phase = 4;
	}
	else if (phase == 4)
	{
		// Remove row
		game_object* rows = get_object_by_name("obj_tetris_rows");
		if (rows != NULL)
		{
			obj_tetris_rows* row_obj = static_cast<obj_tetris_rows*>(rows);

			while (!highlighted_rows.empty())
			{
				//print_debug(to_string(highlighted_rows.size() - 1));
				row_obj->shift_down(highlighted_rows.at(highlighted_rows.size() - 1));
				highlighted_rows.erase(highlighted_rows.begin() + highlighted_rows.size() - 1);
				game.incrementScore(1);
				for (unsigned int i = 0; i < highlighted_rows.size(); i++)
				{
					highlighted_rows[i] += 1;
				}
			}
		}

		phase = 0;
	}
}

// Runs every frame of the subgame whether it's transitioning or not (to draw behind the shade)
void subgame_tetris::subgame_draw()
{
	//printf("Drawing Tetris!!\n");
}

// Clean up subgame objects here, runs once when the game is changing to a different
// game and the transition shade is fully black
void subgame_tetris::subgame_exit()
{
	//printf("Exiting Tetris!!\n");
}

std::string subgame_tetris::subgame_controls_text()
{
	return "D-Pad: Move\nA: Clockwise\nY: Counter-C";
}

subgame_tetris::obj_tetromino::obj_tetromino(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	name = "obj_tetromino";
	angle = 0;
	time_til_drop_move = 60;
	pause_time_drop = 60;
	shape_index = rand() % tetris_shapes.size();
}

void subgame_tetris::obj_tetromino::change_rotation_by(int amount)
{
	angle = (angle + amount + tetris_shapes.at(shape_index).size()) % tetris_shapes.at(shape_index).size();
}

void subgame_tetris::obj_tetromino::check_spots(vector<vector<int>> _potentials, vector<vector<bool>> _sprite, int _direction)
{
	for (unsigned int i = 0; i < _potentials.size(); i++)
	{
		int _x = _potentials.at(i)[0];
		int _y = -_potentials.at(i)[1];

		if (!check_collision(_sprite, x + _x, y + _y))
		{
			change_rotation_by(_direction);
			x += _x;
			y += _y;
			break;
		}
	}
}

void subgame_tetris::obj_tetromino::rotate_piece(bool right)
{
	int iter = (((double)right) - .5) * 2;
	vector<vector<bool>> new_shape = get_sprite(shape_index, angle + iter);

	if (!check_collision(new_shape, x, y))
	{
		change_rotation_by(iter);
	}
	else
	{
		vector<vector<int>> offset_tests;

		// I piece
		if (shape_index == 0)
		{
			if (angle == 0)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2} };
				}
				else
				{
					offset_tests = { {0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1} };
				}
			}
			else if (angle == 1)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1} };
				}
				else
				{
					offset_tests = { {0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2} };
				}
			}
			else if (angle == 2)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2} };
				}
				else
				{
					offset_tests = { {0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1} };
				}
			}
			else if (angle == 3)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1} };
				}
				else
				{
					offset_tests = { {0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2} };
				}
			}
		}
		// not I piece
		else
		{
			if (angle == 0)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} };
				}
				else
				{
					offset_tests = { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} };
				}
			}
			else if (angle == 1)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} };
				}
				else
				{
					offset_tests = { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} };
				}
			}
			else if (angle == 2)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {1, 0}, {1, +1}, {0, -2}, {1, -2} };
				}
				else
				{
					offset_tests = { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} };
				}
			}
			else if (angle == 3)
			{
				if (iter == 1)
				{
					offset_tests = { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} };
				}
				else
				{
					offset_tests = { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} };
				}
			}
		}
		check_spots(offset_tests, new_shape, iter);
	}
}

void subgame_tetris::obj_tetromino::step_function()
{
	if (moving)
	{
		if (keyboard_check_pressed_left(game) || keyboard_check_pressed_right(game) || keyboard_check_pressed_down(game))
			time_til_move = 0;

		if (keyboard_check_pressed_Y(game))
			rotate_piece(false);

		if (keyboard_check_pressed_A(game))
			rotate_piece(true);

		if (keyboard_check_left(game))
			if (time_til_move <= 0)
				move_left();

		if (keyboard_check_right(game))
			if (time_til_move <= 0)
				move_right();

		if (keyboard_check_down(game))
		{
			if (time_til_move <= 0)
			{
				move_down();
				time_til_drop_move = pause_time_drop;
			}
		}

		if (keyboard_check_left(game) || keyboard_check_right(game) || keyboard_check_down(game))
		{
			if (time_til_move <= 0)
				time_til_move = pause_time;
			else
				time_til_move -= 1;
		}

		if (time_til_drop_move > 0)
		{
			time_til_drop_move -= 1;
		}
		else
		{
			time_til_drop_move = pause_time_drop;
			move_down();
		}
	}
}

void subgame_tetris::obj_tetromino::draw_function()
{
	vector<vector<bool>> spr = get_sprite(shape_index, angle);
	place_grid_sprite(game.game_grid, spr, x, y, true);
}

void subgame_tetris::obj_tetromino::destroy_function()
{

}

int subgame_tetris::obj_tetromino::check_off_top(vector<vector<bool>> shape, int _x, int _y)
{
	for (int i = 0; i < grid_width(shape); i++)
	{
		for (int j = 0; j < grid_height(shape); j++)
		{
			if (shape[j][i])
			{
				//print_debug("> "+to_string(_x + i) + " " + to_string(_y + j));
				if (_y + j < 0)
				{
					return 5;
				}
			}
		}
	}

	return 0;
}

// 0 - No Collision
// 1 - Off Board Side Left
// 2 - Off Board Side Right
// 3 - Off Board Bottom
// 4 - Another Piece
int subgame_tetris::obj_tetromino::check_collision(vector<vector<bool>> shape, int _x, int _y)
{
	game_object* rows = get_object_by_name("obj_tetris_rows");

	for (int i = 0; i < grid_width(shape); i++)
	{
		for (int j = 0; j < grid_height(shape); j++)
		{
			if (shape[j][i])
			{
				if (_x + i < 0)
				{
					return 1;
				}

				if (_x + i >= grid_width(game.game_grid))
				{
					return 2;
				}

				if (_y + j >= grid_height(game.game_grid))
				{
					return 3;
				}

				if (rows != NULL)
				{
					obj_tetris_rows* row_obj = static_cast<obj_tetris_rows*>(rows);
					if (grid_get(row_obj->filled_blocks, _x + i, _y + j))
					{
						return 4;
					}
				}
			}
		}
	}

	return 0;
}

vector<vector<bool>> subgame_tetris::obj_tetromino::get_sprite(int _index, int _rotation)
{
	return tetris_shapes.at(_index).at((_rotation + tetris_shapes.at(_index).size()) % tetris_shapes.at(_index).size());
}

void subgame_tetris::obj_tetromino::move_left()
{
	if (!check_collision(get_sprite(shape_index, angle), x - 1, y))
	{
		x -= 1;
	}
}

void subgame_tetris::obj_tetromino::move_right()
{
	if (!check_collision(get_sprite(shape_index, angle), x + 1, y))
	{
		x += 1;
	}
}

void subgame_tetris::obj_tetromino::lose()
{
	game.running = false;
	objects.push_back(std::make_unique<obj_explosion>(game, 5, 0));
}

void subgame_tetris::obj_tetromino::move_down()
{
	if (!check_collision(get_sprite(shape_index, angle), x, y + 1))
	{
		y += 1;
	}
	else
	{
		game_object* rows = get_object_by_name("obj_tetris_rows");
		if (rows != NULL)
		{
			obj_tetris_rows* row_obj = static_cast<obj_tetris_rows*>(rows);

			vector<vector<bool>> gtp = get_sprite(shape_index, angle);
			place_grid_sprite(row_obj->filled_blocks, gtp, x, y);
			//print_debug(to_string(x) + " " + to_string(y));
			if (check_off_top(gtp, x, y))
				lose();
		}
		instance_destroy();
	}
}

subgame_tetris::obj_tetris_rows::obj_tetris_rows(BrickGameFramework& game) : game_object(game, 0, 0)
{
	filled_blocks = grid_create(grid_width(game.game_grid), grid_height(game.game_grid));
	for (int i = 0; i < grid_height(game.game_grid); i++)
	{
		for (int j = 0; j < grid_width(game.game_grid); j++)
		{
			grid_set(game.game_grid, i, j, 0);
		}
	}

	name = "obj_tetris_rows";
}

void subgame_tetris::obj_tetris_rows::step_function()
{

}

void subgame_tetris::obj_tetris_rows::draw_function()
{
	emplace_grid_in_grid(game.game_grid, filled_blocks, x, y, true);
}

void subgame_tetris::obj_tetris_rows::destroy_function()
{

}

void subgame_tetris::obj_tetris_rows::shift_down(int starting_at)
{
	for (int yy = starting_at; yy > 0; yy--)
	{
		for (int xx = 0; xx < grid_width(filled_blocks); xx++)
		{
			grid_set(filled_blocks, xx, yy, grid_get(filled_blocks, xx, yy - 1));
		}
	}
}

void subgame_tetris::obj_tetris_rows::check_rows()
{

}

int subgame_tetris::obj_tetris_rows::lowest_occupied_line(std::vector<std::vector<bool>>& grid)
{
	return 0;
}
