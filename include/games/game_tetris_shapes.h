#pragma once
#include <vector>

vector<vector<bool>> shape_a0
{
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

vector<vector<bool>> shape_a1
{
	{0, 0, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 1, 0}
};

vector<vector<bool>> shape_a2
{
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0}
};

vector<vector<bool>> shape_a3
{
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0}
};

vector<vector<vector<bool>>> shape_a
{
	shape_a0,
	shape_a1,
	shape_a2,
	shape_a3
};

//

vector<vector<bool>> shape_b0
{
	{1, 0, 0},
	{1, 1, 1},
	{0, 0, 0}
};

vector<vector<bool>> shape_b1
{
	{0, 1, 1},
	{0, 1, 0},
	{0, 1, 0}
};

vector<vector<bool>> shape_b2
{
	{0, 0, 0},
	{1, 1, 1},
	{0, 0, 1}
};

vector<vector<bool>> shape_b3
{
	{0, 1, 0},
	{0, 1, 0},
	{1, 1, 0}
};

vector<vector<vector<bool>>> shape_b
{
	shape_b0,
	shape_b1,
	shape_b2,
	shape_b3
};

//


vector<vector<bool>> shape_c0
{
	{0, 0, 1},
	{1, 1, 1},
	{0, 0, 0}
};

vector<vector<bool>> shape_c1
{
	{0, 1, 0},
	{0, 1, 0},
	{0, 1, 1}
};

vector<vector<bool>> shape_c2
{
	{0, 0, 0},
	{1, 1, 1},
	{1, 0, 0}
};

vector<vector<bool>> shape_c3
{
	{1, 1, 0},
	{0, 1, 0},
	{0, 1, 0}
};

vector<vector<vector<bool>>> shape_c
{
	shape_c0,
	shape_c1,
	shape_c2,
	shape_c3
};

//

vector<vector<bool>> shape_d0
{
	{1, 1},
	{1, 1}
};

vector<vector<vector<bool>>> shape_d
{
	shape_d0
};

//

vector<vector<bool>> shape_e0
{
	{0, 1, 1},
	{1, 1, 0},
	{0, 0, 0}
};

vector<vector<bool>> shape_e1
{
	{0, 1, 0},
	{0, 1, 1},
	{0, 0, 1}
};

vector<vector<bool>> shape_e2
{
	{0, 0, 0},
	{0, 1, 1},
	{1, 1, 0}
};

vector<vector<bool>> shape_e3
{
	{1, 0, 0},
	{1, 1, 0},
	{0, 1, 0}
};

vector<vector<vector<bool>>> shape_e
{
	shape_e0,
	shape_e1,
	shape_e2,
	shape_e3
};

//

vector<vector<bool>> shape_f0
{
	{0, 1, 0},
	{1, 1, 1},
	{0, 0, 0}
};

vector<vector<bool>> shape_f1
{
	{0, 1, 0},
	{0, 1, 1},
	{0, 1, 0}
};

vector<vector<bool>> shape_f2
{
	{0, 0, 0},
	{1, 1, 1},
	{0, 1, 0}
};

vector<vector<bool>> shape_f3
{
	{0, 1, 0},
	{1, 1, 0},
	{0, 1, 0}
};

vector<vector<vector<bool>>> shape_f
{
	shape_f0,
	shape_f1,
	shape_f2,
	shape_f3
};

//

vector<vector<bool>> shape_g0
{
	{1, 1, 0},
	{0, 1, 1},
	{0, 0, 0}
};

vector<vector<bool>> shape_g1
{
	{0, 1, 0},
	{1, 1, 0},
	{1, 0, 0}
};

vector<vector<bool>> shape_g2
{
	{0, 0, 0},
	{1, 1, 0},
	{0, 1, 1}
};

vector<vector<bool>> shape_g3
{
	{0, 0, 1},
	{0, 1, 1},
	{0, 1, 0}
};

vector<vector<vector<bool>>> shape_g
{
	shape_g0,
	shape_g1,
	shape_g2,
	shape_g3
};

//

vector<vector<vector<vector<bool>>>> tetris_shapes
{
	shape_a,
	shape_b,
	shape_c,
	shape_d,
	shape_e,
	shape_f,
	shape_g
};