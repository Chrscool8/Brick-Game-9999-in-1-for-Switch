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
	return "D-Pad: Move\nZL/ZR: Fast";
}

subgame_tetris::obj_tetromino::obj_tetromino(BrickGameFramework& game, int _x, int _y) : game_object(game, _x, _y)
{
	name = "obj_tetromino";
	angle = 0;
	time_til_move = 60;
	pause_time = 60;
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

		if (time_til_move > 0)
		{
			time_til_move -= 1;
		}
		else
		{
			time_til_move = pause_time;
			y += 1;
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
	bool can_move = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (shape[j][i])
			{
				if (_x + i < 0)
				{
					can_move = false;
					break;
				}
			}
		}

		if (!can_move)
		{
			break;
		}
	}

	return can_move;
}

vector<vector<bool>> subgame_tetris::obj_tetromino::get_sprite(int index, int rotation)
{
	return tetris_shapes.at(index).at(rotation % tetris_shapes.at(index).size());
}

void subgame_tetris::obj_tetromino::move_left()
{
	if (check_collision(get_sprite(shape_index, angle), x - 1, y))
	{
		x -= 1;
	}
}

void subgame_tetris::obj_tetromino::move_right()
{
	if (check_collision(get_sprite(shape_index, angle), x + 1, y))
	{
		x += 1;
	}
}

subgame_tetris::obj_rows::obj_rows(BrickGameFramework& game, int starting_rows) : game_object(game, 0, 0)
{
	filled_blocks = grid_create(grid_width(game.game_grid), grid_height(game.game_grid));
	for (int i = 0; i < starting_rows; i++)
	{
		for (int j = 0; j < grid_width(game.game_grid); j++)
		{
			grid_set(game.game_grid, i, j, 0);
		}
	}

	name = "obj_rows";
}

void subgame_tetris::obj_rows::step_function()
{
}

void subgame_tetris::obj_rows::draw_function()
{
	emplace_grid_in_grid(game.game_grid, filled_blocks, x, y, true);
}

void subgame_tetris::obj_rows::destroy_function()
{

}