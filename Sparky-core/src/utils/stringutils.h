#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace sparky {

	static std::vector <std::string> split_string(const std::string& s, char delim) {
		std::vector <std::string> elems;
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


}