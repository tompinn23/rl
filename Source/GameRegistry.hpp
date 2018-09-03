#pragma once
#include <string>
#include <map>
#include <set>

#include "spdlog/spdlog.h"

struct Room;
class Map;

class GameRegistry {
public:
	static GameRegistry& get()
	{
		static GameRegistry INSTANCE;
		return INSTANCE;
	};
	void registerRoom(std::string uniqId, Room room);
	void registerMap(std::string uniqId, Map map);
	GameRegistry(GameRegistry const&) = delete;
	void operator=(GameRegistry const&) = delete;
private:
	GameRegistry();
	std::shared_ptr<spdlog::logger> mLog;
	std::set<std::string> ids;
	std::map<std::string, Room> rooms;
	std::map<std::string, Map> maps;

};