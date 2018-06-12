#include "data.hpp"

#include "spdlog/spdlog.h"
#include "sol.hpp"

namespace data {

	rl_room::rl_room(std::string name, int width, int height, std::vector<char> map) :
		name(name), width(width), height(height), map(map)
	{

	}

	rl_room::rl_room(sol::table &room)
	{
		auto log = spdlog::get("main");
		name = room.get<std::string>("name");
		width = room.get<int>("width");
		height = room.get<int>("height");
		sol::table plan = room.get<sol::table>("plan");
		map.resize(height * width);
		log->info("Parsing Room (\"{}\") with size : {}x{}", name, width, height);
		//log->info(height * width);
		for (int j = 1; j <= height; j++)
		{
			//log->info(j);
			//log->info(j - 1);
			std::string line = plan.get<std::string>(j);
			for (int k = 0; k < width; k++)
			{
				//log->info((j -1) *width + k);
				//log->info("{}, {}",k,(j - 1));
				map[k * height + (j - 1)] = line[k];
			}
		}
	}

	char rl_room::get_tile(int x, int y)
	{
		if (x < 0 || y < 0)
			return ' ';
		if (x >= width || y >= height)
			return ' ';
		return map[x * height + y];
	}

	GameData::GameData()
	{

	}

	GameData::~GameData()
	{
		for (auto room : m_rooms)
		{
			delete room;
		}
	}

	bool GameData::init_rooms(sol::table table)
	{
		// Loop over all the rooms.
		for (auto x : table)
		{
			if (x.second.get_type() != sol::type::table)
			{
				//spdlog::get("main")->error("Expected room definition");
				throw sol::error::runtime_error("Room Definition not found");
			}
			sol::table room = x.second.as<sol::table>();
			rl_room* rm = new rl_room(room);
			m_rooms.push_back(rm);
		}
		return true;
	}

	bool GameData::init_data(sol::table table)
	{
		return false;
	}

	rl_room* GameData::get_room(int idx)
	{
		return m_rooms.at(idx);
	}

	void GameData::print_room(int idx)
	{
		rl_room* room = m_rooms[idx];
		spdlog::get("console")->info("Printing room to console:");
		for (int i = 0; i < room->height; i++)
		{
			std::cout << "\t";
			for (int j = 0; j < room->width; j++)
				std::cout << room->get_tile(j, i);
			std::cout << "\n";
		}
	}

	bool init_datafiles(rl_dir data_path, GameData &data)
	{
		auto log = spdlog::get("main");
		log->info("Parsing Data Files");
		sol::state init_data;
		init_data.open_libraries(sol::lib::base);
		init_data["data"] = &data;
		init_data.new_usertype<GameData>("Data", "init_rooms", &GameData::init_rooms);
		for (auto entry : data_path.list_dir()) {
			auto file = std::get_if<rl_file>(&entry);
			if (file)
			{

				sol::protected_function_result result = init_data.safe_script_file(file->get_path(), sol::script_pass_on_error);
				if (!result.valid())
				{
					sol::error err = result;
					sol::call_status status = result.status();
					log->error("So that went wrong:\n{}", err.what());
					return false;
				}
			}
		}
		return true;
	}
}