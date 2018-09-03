#include "data-lua.hpp"
#include "../generate/data.h"

#include <fstream>
#include <sstream>

bool stoint(int &i, char const *s, int base = 0)
{
	char *end;
	long  l;
	errno = 0;
	l = strtol(s, &end, base);
	if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
		return false;
	}
	if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
		return false;
	}
	if (*s == '\0' || *end != '\0') {
		return false;
	}
	i = l;
	return true;
}


void LuaData::addRoom(sol::table room)
{
	Room* sRoom = new Room();
	sRoom->name = room.get<std::string>("name");
	sRoom->width = room.get<int>("width");
	sRoom->height = room.get<int>("height");
	sRoom->colour_map.resize(sRoom->width);
	sRoom->tile_map.resize(sRoom->width);
	for(auto &a : sRoom->colour_map)
	{
		a.resize(sRoom->height);
	}
	for(auto &a: sRoom->tile_map)
	{
		a.resize(sRoom->height);
	}
	if (room.get<bool>("csv"))
	{
		std::ifstream data;
		std::string line;
		std::string working_dir = "";
		std::string path = "./map.csv";
		data.open(working_dir + path);
		if (!data.is_open())
			log->error("Failed to open file \"{}\": {}", working_dir + path, strerror(errno));
		int lNo = 0;
		while (getline(data, line)) {
			// Comment
			if (line[0] == '#')
				continue;
			else {
				std::stringstream ss(line);
				std::vector<std::string> result;
				while (ss.good())
				{
					std::string substr;
					std::getline(ss, substr, ',');
					result.push_back(substr);
				}
				int x = 0;
				int y = 0;
				int c = 0;
				if (!stoint(x, result[0].c_str()))
				{
					log->error("Failed to parse X value at line: {}, file: {}", lNo, working_dir + path);
				}
				if (!stoint(y, result[1].c_str()))
				{
					log->error("Failed to parse Y value at line: {}, file: {}", lNo, working_dir + path);
				}
				if (!stoint(c, result[2].c_str()))
				{
					log->error("Failed to parse character value at line: {}, file: {}", lNo, working_dir + path);
				}
				std::string fgcolour = result[3];
				std::string bgcolour = result[4];
				fgcolour = fgcolour.erase(0, 1);
				bgcolour = bgcolour.erase(0, 1);
				int fr, fg, fb, br, bg, bb;
				bool fail = true;
				fail = stoint(fr, fgcolour.substr(0, 2).c_str(), 16);
				fail = stoint(fg, fgcolour.substr(0, 2).c_str(), 16);
				fail = stoint(fb, fgcolour.substr(0, 2).c_str(), 16);
				if (!fail)
				{
					log->error("Failed to parse hex value at line: {}, file: {}", lNo, working_dir + path);
				}
				uint32_t fore = fr << 24 | fg << 16 | fb << 8 | 255;
				fail = true;
				fail = stoint(br, bgcolour.substr(0, 2).c_str(), 16);
				fail = stoint(bg, bgcolour.substr(0, 2).c_str(), 16);
				fail = stoint(bb, bgcolour.substr(0, 2).c_str(), 16);
				if (!fail)
				{
					log->error("Failed to parse hex value at line: {}, file: {}", lNo, working_dir + path);
				}
				uint32_t back = br << 24 | bg << 16 | bb << 8 | 255;
				sRoom->tile_map[x][y] = c;
				sRoom->colour_map[x][y] = { fore, back };
			}
			lNo++;
		}
		if (data.bad())
			log->error("Error while reading file \"{}\": {}", working_dir + path, strerror(errno));
		data.close();
	}
	else
	{
		//log->info(height * width);
		sol::table plan = room.get<sol::table>("plan");
		for (int j = 1; j <= sRoom->height; j++)
		{
			//log->info(j);
			//log->info(j - 1);
			std::string line = plan.get<std::string>(j);
			for (int k = 0; k < sRoom->width; k++)
			{
				//log->info((j -1) *width + k);
				//log->info("{}, {}",k,(j - 1));
				switch (line[k])
				{
				case '#':
					sRoom->tile_map[k][j] = 1;
					break;
				case '.':
					sRoom->tile_map[k][j] = 2;
					break;
				}

			}
		}
	}
}

void LuaData::addMap(sol::table map)
{
	//TODO: Add map parsing. and therefore add maps!
}
