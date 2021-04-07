#pragma once

#include <string>
#include <vector>

void print_debug(std::string str);
std::string get_config_path();
std::string get_settings_path();
std::string get_scores_path();
std::vector<std::string> explode(std::string const& s, char delim);
void create_directories(std::string path);
double lengthdir_x(double length, double direction_degrees);
double lengthdir_y(double length, double direction_degrees);
