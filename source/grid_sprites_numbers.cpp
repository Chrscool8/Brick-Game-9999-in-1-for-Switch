#include <vector>
#include <grid_sprites_numbers.h>
using namespace std;

vector<vector<bool>> grid_sprite_alphabet_0
{
	{ 0, 1, 1, 0, 0 },
	{ 1, 0, 0, 1, 0 },
	{ 1, 0, 0, 1, 0 },
	{ 1, 0, 0, 1, 0 },
	{ 0, 1, 1, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_1
{
	{ 0, 1, 0, 0, 0 },
	{ 1, 1, 0, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 1, 1, 1, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_2
{
	{ 0, 1, 1, 0, 0 },
	{ 1, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 1, 1, 1, 1, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_3
{
	{ 0, 1, 1, 0, 0 },
	{ 1, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 1, 0, 0, 1, 0 },
	{ 0, 1, 1, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_4
{
	{ 1, 0, 1, 0, 0 },
	{ 1, 0, 1, 0, 0 },
	{ 1, 1, 1, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 0, 1, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_5
{
	{ 1, 1, 1, 0, 0 },
	{ 1, 0, 0, 0, 0 },
	{ 1, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 1, 1, 0, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_6
{
	{ 1, 1, 1, 0, 0 },
	{ 1, 0, 0, 0, 0 },
	{ 1, 1, 1, 0, 0 },
	{ 1, 0, 1, 0, 0 },
	{ 1, 1, 1, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_7
{
	{ 1, 1, 1, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 0, 1, 0, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_8
{
	{ 1, 1, 1, 0, 0 },
	{ 1, 0, 1, 0, 0 },
	{ 1, 1, 1, 0, 0 },
	{ 1, 0, 1, 0, 0 },
	{ 1, 1, 1, 0, 0 }
};

vector<vector<bool>> grid_sprite_alphabet_9
{
	{ 1, 1, 1, 0, 0 },
	{ 1, 0, 1, 0, 0 },
	{ 1, 1, 1, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 1, 1, 0, 0, 0 }
};

vector<vector<bool>> get_gridsprite_number(int i)
{
	switch (i)
	{
	default:
	case 0: return grid_sprite_alphabet_0; break;
	case 1: return grid_sprite_alphabet_1; break;
	case 2: return grid_sprite_alphabet_2; break;
	case 3: return grid_sprite_alphabet_3; break;
	case 4: return grid_sprite_alphabet_4; break;
	case 5: return grid_sprite_alphabet_5; break;
	case 6: return grid_sprite_alphabet_6; break;
	case 7: return grid_sprite_alphabet_7; break;
	case 8: return grid_sprite_alphabet_8; break;
	case 9: return grid_sprite_alphabet_9; break;
	}
}