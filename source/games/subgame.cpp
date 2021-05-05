#include <stdio.h>
#include <game.h>
#include <games/subgame.h>
#include <grid_sprites.h>

subgame::subgame(BrickGameFramework& game_) :parent(game_)
{
	name = "Subgame";
}

void subgame::subgame_init()
{
	printf("Subgame Init\n");
}

void subgame::subgame_step()
{
	printf("Subgame Run\n");
}

void subgame::subgame_draw()
{
	printf("Subgame Draw\n");
}

void subgame::subgame_exit()
{
	printf("Subgame Exit\n");
}

void subgame::subgame_demo()
{
	vector<vector<bool>> sprite;

	if (parent.game_time_in_frames % 120 < 60)
	{
		sprite =
		{
			{ 1, 0, 1 },
			{ 0, 1, 0 },
			{ 1, 0, 1 }
		};
	}
	else
	{
		sprite =
		{
			{ 0, 1, 0 },
			{ 1, 0, 1 },
			{ 0, 1, 0 }
		};
	}

	place_grid_sprite(parent.game_grid, sprite, 5, 5);

	//printf("Subgame Demo\n");
}

std::string subgame::subgame_controls_text()
{
	return "Controls:";
}