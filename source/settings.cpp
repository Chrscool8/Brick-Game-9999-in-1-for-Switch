#include <settings.h>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <utils.hpp>

namespace fs = std::filesystem;

nlohmann::json settings_json;

void read_settings()
{
	if (std::filesystem::exists(get_settings_path()))
	{
		std::ifstream file(get_settings_path());
		settings_json = nlohmann::json::parse(file);
	}
	else
		print_debug("Settings json not found.");
}

void save_settings()
{
	create_directories(get_config_path());
	std::ofstream o(get_settings_path());
	o << settings_json << std::endl;
	print_debug("Saving settings");
}

void settings_set_value(std::string category, std::string key, std::string value)
{
	nlohmann::json j_sub;
	if (settings_json.contains(category))
		j_sub = settings_json[category];

	j_sub[key] = value;
	settings_json[category] = j_sub;
	print_debug("Set " + key + " to " + value);
	save_settings();
}

std::string settings_get_value(std::string category, std::string key)
{
	if (settings_json.contains(category))
	{
		nlohmann::json j_sub = settings_json[category];

		if (j_sub.contains(key))
		{
			if (key != "debug")
				printf(std::string("[BRICKGAME] Reading setting " + category + " " + key + "\n").c_str());
			return (j_sub[key]);
		}
		else
		{
			printf(std::string("[BRICKGAME] Heads up! Setting: " + category + ", >" + key + "< not found.\n").c_str());
			return "---";
		}
	}
	else
	{
		printf(std::string("[BRICKGAME] Heads up! Setting: >" + category + "<, " + key + " not found.\n").c_str());
		return "---";
	}
}

bool settings_get_value_true(std::string category, std::string key)
{
	return (settings_get_value(category, key) == "true");
}

void initialize_setting(std::string category, std::string setting, std::string initial)
{
	if (settings_get_value(category, setting) == "---")
	{
		settings_set_value(category, setting, initial);
	}
}

void init_settings()
{
	settings_set_value("meta", "debug", "true");

	if (settings_get_value("history", "last seen version") == "---" || (APP_VERSION != settings_get_value("history", "last seen version")))
	{
		settings_set_value("history", "last seen version", APP_VERSION);
		print_debug("DIFFERING VERSION!!");
	}
}
