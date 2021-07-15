#include <platform/control_layer.h>
#include <platform/switch/control_layer_switch.h>

void init_controllers() { init_controllers_switch(); }
void update_controller() { update_controllers_switch(); }

bool keyboard_check_up() { return keyboard_check_up_switch(); }
bool keyboard_check_right() { return keyboard_check_right_switch(); }
bool keyboard_check_left() { return keyboard_check_left_switch(); }
bool keyboard_check_down() { return keyboard_check_down_switch(); }

bool keyboard_check_L() { return keyboard_check_L_switch(); }
bool keyboard_check_R() { return keyboard_check_R_switch(); }
bool keyboard_check_ZL() { return keyboard_check_ZL_switch(); }
bool keyboard_check_ZR() { return keyboard_check_ZR_switch(); }

bool keyboard_check_A() { return keyboard_check_A_switch(); }
bool keyboard_check_B() { return keyboard_check_B_switch(); }
bool keyboard_check_X() { return keyboard_check_X_switch(); }
bool keyboard_check_Y() { return keyboard_check_Y_switch(); }

bool keyboard_check_pressed_right() { return keyboard_check_pressed_right_switch(); }
bool keyboard_check_pressed_up() { return keyboard_check_pressed_up_switch(); }
bool keyboard_check_pressed_left() { return keyboard_check_pressed_left_switch(); }
bool keyboard_check_pressed_down() { return keyboard_check_pressed_down_switch(); }

bool keyboard_check_pressed_A() { return keyboard_check_pressed_A_switch(); }
bool keyboard_check_pressed_B() { return keyboard_check_pressed_B_switch(); }
bool keyboard_check_pressed_X() { return keyboard_check_pressed_X_switch(); }
bool keyboard_check_pressed_Y() { return keyboard_check_pressed_Y_switch(); }

bool keyboard_check_pressed_L() { return keyboard_check_pressed_L_switch(); }
bool keyboard_check_pressed_R() { return keyboard_check_pressed_R_switch(); }

bool keyboard_check_pressed_start() { return keyboard_check_pressed_start_switch(); }
bool keyboard_check_pressed_select() { return keyboard_check_pressed_select_switch(); }