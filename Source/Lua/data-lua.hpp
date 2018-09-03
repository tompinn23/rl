#pragma once
#include "sol.hpp"
#include "spdlog/spdlog.h"

class LuaData {
private:
	std::string working_dir;
	spdlog::logger* log;
public:
	void addRoom(sol::table room);
	void addMap(sol::table map);
};

