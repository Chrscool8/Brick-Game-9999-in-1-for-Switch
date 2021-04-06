#include <stdio.h>
#include <games/subgame.h>

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
