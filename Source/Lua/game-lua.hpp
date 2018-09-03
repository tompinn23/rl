#pragma once
#include "sol.hpp"

class Player;

class LuaGame {
	Player* player;
	void onTick();
	//etc.
};