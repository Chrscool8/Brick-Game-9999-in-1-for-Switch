#pragma once
void init_controllers();
void update_controller();

bool keyboard_check_up();
bool keyboard_check_right();
bool keyboard_check_left();
bool keyboard_check_down();

bool keyboard_check_L();
bool keyboard_check_R();
bool keyboard_check_ZL();
bool keyboard_check_ZR();

bool keyboard_check_A();
bool keyboard_check_B();
bool keyboard_check_X();
bool keyboard_check_Y();

bool keyboard_check_pressed_right();
bool keyboard_check_pressed_up();
bool keyboard_check_pressed_left();
bool keyboard_check_pressed_down();

bool keyboard_check_pressed_A();
bool keyboard_check_pressed_B();
bool keyboard_check_pressed_X();
bool keyboard_check_pressed_Y();

bool keyboard_check_pressed_L();
bool keyboard_check_pressed_R();

bool keyboard_check_pressed_start();
bool keyboard_check_pressed_select();