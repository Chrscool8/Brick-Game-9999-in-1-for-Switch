#include <stdio.h>
#include <vector>
#include <grid.hpp>
#include <functional>
#include <memory>
using namespace std;

struct point {
	int x;
	int y;
};

class game_object
{
public:
	static unsigned int object_index;
	unsigned int id;
	enum {
		direction_right,
		direction_up,
		direction_left,
		direction_down
	};

	point position = point{ 5, 5 };
	int direction = direction_right;

	vector<vector<bool>>& gg;

	game_object(vector<vector<bool>>& arr) : gg(arr)
	{
		id = object_index;
		object_index += 1;
		create_function();
	}

	vector<vector<bool>> sprite;
	virtual void create_function()
	{
		printf("ID: %u\n", id);
		printf("Obj create\n");
	};
	virtual void step_function()
	{
		printf("ID: %u\n", id);
		printf("Obj step\n");
	};
	virtual void draw_function()
	{
		printf("ID: %u\n", id);
		printf("Obj draw\n");
	};
	virtual void destroy_function()
	{
		printf("ID: %u\n", id);
		printf("Obj destroy\n");
	};
};

unsigned int game_object::object_index = 0;

class obj_snake : public game_object
{
public:

	int move_counter;
	obj_snake(vector<vector<bool>>& arr) :game_object(arr)
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
				position.x += 1;
				break;
			case direction_up:
				position.y -= 1;
				break;
			case direction_left:
				position.x -= 1;
				break;
			case direction_down:
				position.y += 1;
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
		printf("%i, %i\n", position.x, position.y);
		printf("%p\n", (void*)&gg);
		grid_set(gg, position.x, position.y, true);
	};
	virtual void destroy_function() override
	{
		printf("ID: %u\n", id);
		printf("Snake destroy\n");
	};
};

vector<std::unique_ptr<game_object>> objects;

void game_snake_init(vector<vector<bool>>& game_grid)
{
	printf("Initting Snake!!\n");
	printf("%p\n", (void*)&game_grid);
	objects.push_back(std::make_unique<obj_snake>(game_grid));
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

