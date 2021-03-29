#include <stdio.h>
#include <subgame.h>

subgame::subgame(BrickGameFramework& game_) :parent(game_)
{
}

void subgame::subgame_init()
{
	printf("Subgame Init\n");
}

void subgame::subgame_run()
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

