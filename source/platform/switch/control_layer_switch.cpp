#include <platform/switch/control_layer_switch.h>
#include <switch.h>

class control_lib
{
public:
	PadState pad;
};

control_lib CTL;

void init_controllers_switch()
{
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&CTL.pad);
}

void update_controllers_switch()
{
	padUpdate(&CTL.pad);
	// u64 keyboard_check_pressed = padGetButtonsDown(&pad);
	// u64 keyboard_check = padGetButtons(&pad);
}

bool keyboard_check_right_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_Right);
}

bool keyboard_check_up_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_Up);
}

bool keyboard_check_left_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_Left);
}

bool keyboard_check_down_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_Down);
}

bool keyboard_check_L_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_L);
}

bool keyboard_check_R_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_R);
}

bool keyboard_check_ZL_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_ZL);
}

bool keyboard_check_ZR_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_ZR);
}

bool keyboard_check_A_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_A);
}

bool keyboard_check_B_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_B);
}

bool keyboard_check_X_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_X);
}

bool keyboard_check_Y_switch()
{
	u64 keyboard_check = padGetButtons(&CTL.pad);
	return (keyboard_check & HidNpadButton_Y);
}

//

bool keyboard_check_pressed_right_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_Right);
}

bool keyboard_check_pressed_up_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_Up);
}

bool keyboard_check_pressed_left_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_Left);
}

bool keyboard_check_pressed_down_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_Down);
}

bool keyboard_check_pressed_A_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_A);
}

bool keyboard_check_pressed_B_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_B);
}

bool keyboard_check_pressed_X_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_X);
}

bool keyboard_check_pressed_Y_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_Y);
}

bool keyboard_check_pressed_L_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_L);
}

bool keyboard_check_pressed_R_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_R);
}

bool keyboard_check_pressed_start_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_Plus);
}

bool keyboard_check_pressed_select_switch()
{
	u64 keyboard_check = padGetButtonsDown(&CTL.pad);
	return (keyboard_check & HidNpadButton_Minus);
}