#include <game.h>

bool keyboard_check_right(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Right);
}

bool keyboard_check_up(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Up);
}

bool keyboard_check_left(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Left);
}

bool keyboard_check_down(BrickGame& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Down);
}