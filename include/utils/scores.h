#pragma once
#include <string>

void read_scores();
void save_scores();
void scores_set_score_value(std::string category, std::string game, std::string value);
std::string scores_get_score_value(std::string category, std::string key);