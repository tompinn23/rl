#pragma once

#include <map>
#include <vector>
#include <set>

#include "enums.h"
#include "spdlog/spdlog.h"

struct ColourPair {
	uint32_t fg;
	uint32_t bg;
};

struct Room {
	std::string name;
	int width;
	int height;
	std::vector<std::vector<int>> tile_map;
	std::vector<std::vector<ColourPair>> colour_map;

};
class Map
{

};


