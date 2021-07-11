#pragma once
#include <vector>

vector<vector<bool>> shape_a0
{
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0}
};

vector<vector<bool>> shape_a1
{
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

vector<vector<vector<bool>>> shape_a
{
	shape_a0,
	shape_a1
};

//

vector<vector<bool>> shape_b0
{
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{1, 1, 0, 0},
	{0, 0, 0, 0}
};

vector<vector<bool>> shape_b1
{
	{1, 0, 0, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

vector<vector<bool>> shape_b2
{
	{1, 1, 0, 0},
	{1, 0, 0, 0},
	{1, 0, 0, 0},
	{0, 0, 0, 0}
};

vector<vector<bool>> shape_b3
{
	{1, 1, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};


vector<vector<vector<bool>>> shape_b
{
	shape_b0,
	shape_b1,
	shape_b2,
	shape_b3
};

//

vector<vector<vector<vector<bool>>>> tetris_shapes
{
	shape_a,
	shape_b
};