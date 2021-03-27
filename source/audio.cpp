#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "audio.h"
#include <vector>
#include <map>
#include <string>

std::map<std::string, Mix_Chunk*> audio_files;
Mix_Music* music;

bool init_audio()
{
	printf("SDL stuff\n");
	// Start SDL with audio support
	int i = SDL_Init(SDL_INIT_AUDIO);
	printf("%i\n", i);

	// Load support for the MP3 format
	i = Mix_Init(MIX_INIT_MP3);
	printf("%i\n", i);

	// open 44.1KHz, signed 16bit, system byte order,
	//  stereo audio, using 4096 byte chunks
	i = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
	printf("%i\n", i);

	// Load sound file to use
	audio_files["sfx_movement_footsteps5"] = Mix_LoadWAV("romfs:/audio/sfx_movement_footsteps5.wav");
	audio_files["sfx_exp_odd3"] = Mix_LoadWAV("romfs:/audio/sfx_exp_odd3.wav");
	audio_files["sfx_sounds_button6"] = Mix_LoadWAV("romfs:/audio/sfx_sounds_button6.wav");

	music = Mix_LoadMUS("romfs:/audio/chipscape.mp3");
	Mix_PlayMusic(music, -1);

	return false;
}

bool exit_audio()
{
	for (auto const& [key, val] : audio_files)
	{
		Mix_FreeChunk(val);
	}

	Mix_FreeMusic(music);

	// Shuts down SDL subsystems
	SDL_Quit();

	return false;
}
