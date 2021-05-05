#include <stdio.h>
#include <game.h>
#include <games/game_template.h>

subgame_template::subgame_template(BrickGameFramework& _parent) : subgame(_parent)
{
	name = "Template";
}

void subgame_template::subgame_init()
{
	if (parent.debug_text)
		printf("Initting Template!!\n");
}

void subgame_template::subgame_step()
{
}

void subgame_template::subgame_draw()
{
}

void subgame_template::subgame_exit()
{
	if (parent.debug_text)
		printf("Exiting Template!!\n");
}

std::string subgame_template::subgame_controls_text()
{
	return "";
}
