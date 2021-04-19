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

bool keyboard_check_L(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtons(&game.pad);
	return (keyboard_check & HidNpadButton_L);
}

//

bool keyboard_check_pressed_right(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtonsDown(&game.pad);
	return (keyboard_check & HidNpadButton_Right);
}

bool keyboard_check_pressed_up(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtonsDown(&game.pad);
	return (keyboard_check & HidNpadButton_Up);
}

bool keyboard_check_pressed_left(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtonsDown(&game.pad);
	return (keyboard_check & HidNpadButton_Left);
}

bool keyboard_check_pressed_down(BrickGameFramework& game)
{
	u64 keyboard_check = padGetButtonsDown(&game.pad);
	return (keyboard_check & HidNpadButton_Down);
}