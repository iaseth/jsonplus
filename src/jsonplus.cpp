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
		std::cout << "Too few arguments!\n";
		return 0;
	}

	std::string filename = argv[1];
	if (!fs::exists(filename))
	{
		std::cout << "File not found: " << filename << "\n";
		return 0;
	}

	try
	{
		std::ifstream jfs(filename);
		auto jo = nj::json::parse(jfs);
		std::cout << jo.dump('\t') << "\n";
	}
	catch (...)
	{
		std::cout << "Could not parse: " << filename << "\n";
		return 0;
	}

	return 0;
}
