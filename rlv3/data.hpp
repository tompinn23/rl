#ifndef DATA_H
#define DATA_H

#include "top-level.hpp"

#include "sol.hpp"
#include <vector>
#include <string>

#include "u-util.hpp"
#include "u-file.hpp"

namespace data {

	struct rl_room {
		std::string name;
		int width;
		int height;
		std::vector<char> map;
		rl_room(std::string name, int width, int height, std::vector<char> map);
		rl_room(sol::table &room);
		char get_tile(int x, int y);
	};




	class GameData {
	public:
		GameData();
		~GameData();
		bool init_rooms(sol::table table);
		bool init_data(sol::table table);
		void print_room(int idx);
		rl_room* get_room(int idx);
	private:
		std::vector<rl_room*> m_rooms;
	};


	bool init_datafiles(rl_dir data_path, GameData &data);
}
#endif
