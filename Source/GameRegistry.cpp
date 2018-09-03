#include "GameRegistry.hpp"
#include "generate/data.h"

GameRegistry::GameRegistry()
{
	mLog = spdlog::get("main");
}

void GameRegistry::registerRoom(std::string uniqId, Room room)
{
	if (ids.insert(uniqId).second)
	{
		rooms.emplace(uniqId, room);
	}
	else {
		mLog->error("Failed to add room with id: {} as it already exists.");
	}
}

void GameRegistry::registerMap(std::string uniqId, Map map)
{
	if (ids.insert(uniqId).second)
	{
		maps.emplace(uniqId, map);
	}
	else {
		mLog->error("Failed to add map with id: {} as it already exists.");
	}
}