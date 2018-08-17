#pragma once

#include <map>
#include <vector>
#include "enums.h"


struct Room {
	std::string name;
	int width;
	int height;
	std::vector<std::vector<Tile>> tile_map;

};
class Map;


class GameRegistry {
public:
	static GameRegistry* get();
	void registerRoom(Room room);
	void registerMap(Map map);
private:
	static GameRegistry INSTANCE = nullptr;
	GameRegistry();

	std::map<std::string, Room> rooms;
	std::map<std::string, Map> maps;

};