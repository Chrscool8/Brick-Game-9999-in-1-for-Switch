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
	//printf("Running Tetris!!\n");
	if (keyboard_check_pressed_A(game))
	{
		objects.push_back(std::make_unique<obj_tetromino>(game, grid_width(game.game_grid) / 2 - 2, 0));
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
	return "Left/Right: Move\nUp: Rotate";
}

subgame_tetris::obj_tetromino::obj_tetromino(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	name = "obj_tetromino";
	angle = 0;
	time_til_drop_move = 60;
	pause_time_drop = 60;
	shape_index = rand() % tetris_shapes.size();
}

void subgame_tetris::obj_tetromino::step_function()
{
	if (moving)
	{
		if (keyboard_check_pressed_B(game))
			instance_destroy();

		if (keyboard_check_pressed_up(game))
			angle += 1;

		if (keyboard_check_pressed_left(game))
			move_left();

		if (keyboard_check_pressed_right(game))
			move_right();

		if (keyboard_check_pressed_down(game))
		{
			move_down();
			time_til_drop_move = pause_time_drop;
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
	place_grid_sprite(game.game_grid, tetris_shapes.at(shape_index).at(angle % tetris_shapes.at(shape_index).size()), x, y);
}

void subgame_tetris::obj_tetromino::destroy_function()
{

}

bool subgame_tetris::obj_tetromino::check_collision(vector<vector<bool>> shape, int _x, int _y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (shape[j][i])
			{
				if (_x + i < 0 || _x + i >= grid_width(game.game_grid) || _y + j >= grid_height(game.game_grid))
				{
					return true;
				}

				game_object* rows = get_object_by_name("obj_tetris_rows");
				if (rows != NULL)
				{
					obj_tetris_rows* row_obj = static_cast<obj_tetris_rows*>(rows);
					if (grid_get(row_obj->filled_blocks, _x + i, _y + j))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

vector<vector<bool>> subgame_tetris::obj_tetromino::get_sprite(int index, int rotation)
{
	return tetris_shapes.at(index).at(rotation % tetris_shapes.at(index).size());
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
			place_grid_sprite(row_obj->filled_blocks, gtp, x, y, true);
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

void subgame_tetris::obj_tetris_rows::shift_down()
{
}

void subgame_tetris::obj_tetris_rows::check_rows()
{
}

int subgame_tetris::obj_tetris_rows::lowest_occupied_line(std::vector<std::vector<bool>>& grid)
{
	return 0;
}
