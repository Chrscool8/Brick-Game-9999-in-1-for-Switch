#pragma once
#ifndef CONTROLS_H
#define CONTROLS_H

#include <game.h>

bool keyboard_check_right(BrickGameFramework& game);
bool keyboard_check_up(BrickGameFramework& game);
bool keyboard_check_left(BrickGameFramework& game);
bool keyboard_check_down(BrickGameFramework& game);
bool keyboard_check_L(BrickGameFramework& game);

bool keyboard_check_pressed_right(BrickGameFramework& game);
bool keyboard_check_pressed_up(BrickGameFramework& game);
bool keyboard_check_pressed_left(BrickGameFramework& game);
bool keyboard_check_pressed_down(BrickGameFramework& game);

bool keyboard_check_pressed_A(BrickGameFramework& game);
bool keyboard_check_pressed_B(BrickGameFramework& game);
bool keyboard_check_pressed_X(BrickGameFramework& game);
bool keyboard_check_pressed_Y(BrickGameFramework& game);

#endif // !CONTROLS_H