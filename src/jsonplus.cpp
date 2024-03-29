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
		if (keys.size() == 0) {
			// display whole JSON when no keys are supplied
			std::cout << jo.dump('\t') << "\n";
		} else {
			for (auto key : keys) {
				if (jo.is_object()) {
					if (jo.contains(key)) {
						jo = jo[key];
					} else {
						// key was not found on the object
						std::cout << "Key NOT found on object: '" << key << "'\n";
						std::cout << "Object: {" << jo.size() << " keys}\n";
						for (auto it = jo.begin(); it != jo.end(); ++it) {
							std::cout << "\tkey: '" << it.key() << "'\n";
						}
						return 0;
					}
				} else if (jo.is_array()) {
					try {
						int index = std::stoi(key);
						if (index < 0) {
							// allowing negative index
							index += jo.size();
						}

						if (index < 0 || index >= jo.size()) {
							// index outside [0, jo.size())
							std::cout << "Index out out range: " << index << " [" << jo.size() << " elements]\n";
							return 0;
						}
						jo = jo[index];
					} catch (...) {
						// when index is not an integer
						std::cout << "Invalid index for array: '" << key << "'\n";
						std::cout << "Array: [" << jo.size() << " elements]\n";
						return 0;
					}
				} else {
					// jo is a primitive value
					std::cout << "Too many keys provided: '" << key << "'\n";
					std::cout << "Primitive value: " << jo.dump('\t') << "\n";
					return 0;
				}
			}
			// all keys were found
			std::cout << jo.dump('\t') << "\n";
		}
	} catch (...) {
		std::cout << "Could not parse: '" << filename << "'\n";
		return 0;
	}

	return 0;
}
