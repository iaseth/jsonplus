#include <iostream>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
namespace nj = nlohmann;

int main (int argc, char const *argv[])
{
	if (argc == 1)
	{
		std::cout << "Usage: json++ <filename>\n";
		return 0;
	}

	std::string filename = argv[1];
	if (!fs::exists(filename))
	{
		std::cout << "File NOT found: '" << filename << "'\n";
		return 0;
	}

	std::vector<std::string> keys;
	if (argc > 2)
	{
		for (int i = 2; i < argc; ++i)
		{
			std::string key = argv[i];
			keys.push_back(key);
		}
	}

	try
	{
		std::ifstream jfs(filename);
		auto jo = nj::json::parse(jfs);
		if (keys.size() == 0)
		{
			// display whole JSON when no keys are supplied
			std::cout << jo.dump('\t') << "\n";
		}
		else
		{
			for (auto key : keys)
			{
				if (jo.contains(key))
				{
					jo = jo[key];
				}
				else
				{
					std::cout << "Key NOT found: '" << key << "'\n";
					std::cout << jo.dump('\t');
					return 0;
				}
			}
			// all keys were found
			std::cout << jo.dump('\t') << "\n";
		}
	}
	catch (...)
	{
		std::cout << "Could not parse: '" << filename << "'\n";
		return 0;
	}

	return 0;
}
