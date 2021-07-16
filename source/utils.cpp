#include <string>
#include <settings.h>
#include <nanovg.h>

void print_debug(std::string str)
{
	if (settings_get_value_true("meta", "debug"))
	{
		str = "[BRICKGAME] " + str + "\n";
		printf(str.c_str());
	}
}

std::string get_config_path()
{
	return "sdmc:/config/brickgame/";
}

std::string get_settings_path()
{
	return get_config_path() + "settings.json";
}

std::string get_scores_path()
{
	return get_config_path() + "scores.json";
}

std::vector<std::string> explode(std::string const& s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim);)
	{
		result.push_back(std::move(token));
	}

	return result;
}

void create_directories(std::string path)
{
	if (!fs::exists(path))
	{
		std::vector<std::string> folders = explode(path, '/');
		std::string subpath = "";
		for (unsigned int i = 0; i < folders.size(); i++)
		{
			subpath += folders.at(i) + "/";
			print_debug(subpath);
			if (!fs::exists(subpath))
				fs::create_directory(subpath);
		}
	}
}

double lengthdir_x(double length, double direction_degrees)
{
	return length * cos(nvgDegToRad(direction_degrees));
}

double lengthdir_y(double length, double direction_degrees)
{
	return length * -sin(nvgDegToRad(direction_degrees));
}

#define PI 3.14159265358979323846264338327f

float DegToRad(float deg)
{
	return deg / 180.0f * PI;
}

float RadToDeg(float rad)
{
	return rad / PI * 180.0f;
}