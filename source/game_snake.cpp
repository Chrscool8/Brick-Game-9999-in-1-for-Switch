#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
#include <object_manager.h>
using namespace std;

class obj_snake : public game_object
{
public:
	int move_counter;
	obj_snake(vector<vector<bool>>& arr, int _x, int _y) :game_object(arr, _x, _y)
	{
		create_function();
	};

	virtual void create_function() override
	{
		printf("ID: %u\n", id);
		printf("Snake create\n");

		move_counter = 0;
	};
	virtual void step_function() override
	{
		printf("ID: %u\n", id);
		printf("Snake step\n");

		if (move_counter < 50)
		{
			move_counter += 1;
		}
		else
		{
			move_counter = 0;

			switch (direction)
			{
			case direction_right:
				x += 1;
				break;
			case direction_up:
				y -= 1;
				break;
			case direction_left:
				x -= 1;
				break;
			case direction_down:
				y += 1;
				break;
			default:
				printf("No direction set\n");
			}
		}
	};
	virtual void draw_function() override
	{
		printf("ID: %u\n", id);
		printf("Snake draw\n");
		printf("%i, %i\n", x, y);
		printf("%p\n", (void*)&gg);
		grid_set(gg, x, y, true);
	};
	virtual void destroy_function() override
	{
		printf("ID: %u\n", id);
		printf("Snake destroy\n");
	};
};


void game_snake_init(vector<vector<bool>>& game_grid)
{
	printf("Initting Snake!!\n");
	printf("%p\n", (void*)&game_grid);
	objects.push_back(std::make_unique<obj_snake>(game_grid, 5, 5));
}

void game_snake_run(vector<vector<bool>>& game_grid)
{
	printf("Running Snake!!\n");

	grid_clear(game_grid);

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects.at(i)->step_function();
	}

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects.at(i)->draw_function();
	}
}

void game_snake_exit(vector<vector<bool>>& game_grid)
{
	printf("Exiting Snake!!\n");
}

