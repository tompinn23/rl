#include "catch.hpp"
#include "data.hpp"
#include "sol.hpp"

TEST_CASE("Room definition is correctly parsed", "[datafiles]") {
	sol::state lua;
	data::GameData dat = data::GameData();
	lua.open_libraries(sol::lib::base);
	lua["data"] = &dat;
	lua.new_usertype<data::GameData>("Data", "init_rooms", &data::GameData::init_rooms);
	std::string script = R"(
	data:init_rooms({
	{
		name="Room 1",
		width=6,
		height=7,
		plan={
			"######",
			"#....#",
			"#....#",
			"#....#",
			"#....#",
			"#....#",
			"######"
		}
	}}))";
	auto result = lua.safe_script(script, sol::script_pass_on_error);
	if (!result.valid())
	{
		sol::error err = result;
		sol::call_status status = result.status();
		std::cerr << "So that went wrong:\n" << err.what();
	}
	SECTION("Script parses correctly") {
		REQUIRE(result.valid());
	}
	SECTION("Room name is parsed correctly") {
		CHECK(dat.get_room(0)->name == "Room 1");
	}
	SECTION("Width is parsed correctly") {
		CHECK(dat.get_room(0)->width == 6);
	}
	SECTION("Height is parsed correctly") {
		CHECK(dat.get_room(0)->height == 7);
	}
	SECTION("Template is correctly parsed") {
		std::string rows[7] = { "######", "#....#", "#....#",	"#....#", "#....#", "#....#", "######" };
		auto room = dat.get_room(0);
		for (int i = 0; i < room->height; i++)
		{
			std::string row;
			for (int j = 0; j < room->width; j++)
			{
				row += (room->get_tile(j, i));
			}
			CHECK(row == rows[i]);
		}
	}
}