#include "data.h"

GameRegistry * GameRegistry::get()
{
	if (INSTANCE == nullptr)
		INSTANCE = GameRegistry();
	return INSTANCE;
}

void GameRegistry::registerRoom(Room room)
{
}

void GameRegistry::registerMap(Map map)
{

}
