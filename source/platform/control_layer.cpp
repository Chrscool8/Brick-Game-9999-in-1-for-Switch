#include <platform/switch/control_layer_switch.h>
#include <platform/switch/graphics_layer_switch.h>
#include <platform/control_layer.h>

void init_controllers() { init_controllers_switch(); }
void update_controller() { update_controllers_switch(); }

bool keyboard_check_up() { keyboard_check_up_switch(); }
bool keyboard_check_right() { keyboard_check_right_switch(); }
bool keyboard_check_left() { keyboard_check_left_switch(); }
bool keyboard_check_down() { keyboard_check_down_switch(); }

bool keyboard_check_L() { keyboard_check_L_switch(); }
bool keyboard_check_R() { keyboard_check_R_switch(); }
bool keyboard_check_ZL() { keyboard_check_ZL_switch(); }
bool keyboard_check_ZR() { keyboard_check_ZR_switch(); }

bool keyboard_check_A() { keyboard_check_A_switch(); }
bool keyboard_check_B() { keyboard_check_B_switch(); }
bool keyboard_check_X() { keyboard_check_X_switch(); }
bool keyboard_check_Y() { keyboard_check_Y_switch(); }

bool keyboard_check_pressed_right() { keyboard_check_pressed_right_switch(); }
bool keyboard_check_pressed_up() { keyboard_check_pressed_up_switch(); }
bool keyboard_check_pressed_left() { keyboard_check_pressed_left_switch(); }
bool keyboard_check_pressed_down() { keyboard_check_pressed_down_switch() }

bool keyboard_check_pressed_A() { keyboard_check_pressed_A_switch(); }
bool keyboard_check_pressed_B() { keyboard_check_pressed_B_switch(); }
bool keyboard_check_pressed_X() { keyboard_check_pressed_X_switch(); }
bool keyboard_check_pressed_Y() { keyboard_check_pressed_Y_switch(); }

bool keyboard_check_pressed_L() { keyboard_check_pressed_L_switch(); }
bool keyboard_check_pressed_R() { keyboard_check_pressed_R_switch(); }

bool keyboard_check_pressed_start() { keyboard_check_pressed_start_switch(); }
bool keyboard_check_pressed_select() { keyboard_check_pressed_select_switch(); }