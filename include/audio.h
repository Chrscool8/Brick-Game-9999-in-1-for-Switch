#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

extern std::map<std::string, Mix_Chunk*> audio_files;

bool init_audio();
bool exit_audio();