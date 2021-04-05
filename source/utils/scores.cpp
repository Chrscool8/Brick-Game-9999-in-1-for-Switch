#include <utils/base64.h>
#include <utils/scores.h>
#include <utils.hpp>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

nlohmann::json scores_json;

void read_scores()
{
	if (std::filesystem::exists(get_scores_path()))
	{
		std::ifstream i(get_scores_path());
		scores_json.clear();
		i >> scores_json;
	}
}

void save_scores()
{
	std::ofstream o(get_scores_path());
	o << scores_json << std::endl;
}

void scores_set_score_value(std::string category, std::string game, std::string value)
{
	//std::string category = "score";

	nlohmann::json j_sub;
	if (scores_json.contains(category))
		j_sub = scores_json[category];

	j_sub[game] = value;
	scores_json[category] = j_sub;
	print_debug("Set " + game + " score to " + value);

	save_scores();
}

std::string scores_get_score_value(std::string category, std::string key)
{
	//std::string category = "score";
	if (scores_json.contains(category))
	{
		nlohmann::json j_sub = scores_json[category];

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