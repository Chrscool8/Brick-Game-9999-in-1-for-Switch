#include <controls.h>
#include <game.h>

bool keyboard_check_right(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Right);
}

bool keyboard_check_up(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Up);
}

bool keyboard_check_left(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Left);
}

bool keyboard_check_down(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_Down);
}