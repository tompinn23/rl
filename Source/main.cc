
#include <iostream>
#include <chrono>
#include <bitset>
#include <fstream>
#include <sstream>

//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "spdlog/spdlog.h"

#include "platform.h"

#include "magma/Console.h"
#include "magma/input.h"
#include "generate/generate.h"
#include "generate/enums.h"

using magma::Console;
using magma::CommandManager;
using namespace magma;

void init_logging()
{
	try
	{
		std::vector<spdlog::sink_ptr> sinks;
#if defined(Windows)
		sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
		sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
#endif
		sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("rlv3.log"));
		auto console_logger = std::make_shared<spdlog::logger>("console", sinks[0]);
		auto main_logger = std::make_shared<spdlog::logger>("main", begin(sinks), end(sinks));
		spdlog::register_logger(console_logger);
		spdlog::register_logger(main_logger);
		console_logger->set_level(spdlog::level::info);
		main_logger->set_level(spdlog::level::info);
		console_logger->info("Testing console logger!");
		main_logger->info("Testing combined logger!");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log initialization failed: " + std::string(ex.what()) + "\n";
		exit(EXIT_FAILURE);
	}
}

bool stoint(int &i, char const *s, int base = 0);


int main(int argc, char* argv[])
{
	init_logging();
	auto log = spdlog::get("main");
	log->info("Creating Screen.");
	magma::init();
	auto root = Console::init_root(100, 100, "Taffer_10x10.png", 10);
	auto con = Console::init(40, 20);

	int num = SDL_GetNumRenderDrivers();
	SDL_RendererInfo info;
	for (int i = 0; i < num; i++)
	{
		SDL_GetRenderDriverInfo(i, &info);
		log->info("Driver: {} is {}", i, info.name);
	}
	CommandManager rlCommands = CommandManager();
	//					  INPUT DOMAIN				                     COMMAND	    	NAME(uniq)	IS KB	KEYCODE			PRESSED,SHIFT,CTRL,ALT,CAPS	
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_E,		"move_e",	true ,	SDLK_RIGHT,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_N,		"move_n",	true ,	SDLK_UP,		true ,false,false,false,false));	
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_NE,		"move_ne",	true ,	SDLK_RIGHT,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_W,		"move_w",	true ,	SDLK_LEFT,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_NW,		"move_nw",	true ,	SDLK_LEFT,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_S,		"move_s",	true ,	SDLK_DOWN,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_SE,		"move_se",	true ,	SDLK_RIGHT,		true ,false,true ,false,false));	
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_SW,		"move_sw",	true ,	SDLK_LEFT,		true ,false,true ,false,false));

	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_SPACE_K,		"txt_spc",	true ,	SDLK_SPACE,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_EXCL_K,		"txt_!",	true ,	SDLK_1,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_QUOTE_K,		"txt_\"",	true ,	SDLK_2,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_HASH_K,		"txt_#",	true ,	SDLK_HASH,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_DOLLAR_K,		"txt_$",	true ,	SDLK_4,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_PERCENT_K,		"txt_%",	true ,	SDLK_5,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_AMPERSAND_K,	"txt_&",	true ,	SDLK_7,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_APOSTROPHE_K,	"txt_'",	true ,	SDLK_QUOTE,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_LEFT_PAREN_K,	"txt_(",	true ,	SDLK_9,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_RIGHT_PAREN_K,	"txt_)",	true ,	SDLK_0,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_ASTERIX_K,		"txt_*",	true ,	SDLK_8,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_PLUS_K,		"txt_=",	true ,	SDLK_EQUALS,	true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_COMMA_K,		"txt_,",	true ,	SDLK_COMMA,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_MINUS_K,		"txt_-",	true ,	SDLK_MINUS,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_STOP_K,		"txt_.",	true ,	SDLK_PERIOD,	true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_SLASH_K,		"txt_/",	true ,	SDLK_SLASH,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_0_K,			"txt_0",	true ,	SDLK_0,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_1_K,			"txt_1",	true ,	SDLK_1,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_2_K,			"txt_2",	true ,	SDLK_2,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_3_K,			"txt_3",	true ,	SDLK_3,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_4_K,			"txt_4",	true ,	SDLK_4,			true ,false,false,false,false));





	log->info("{}", sizeof(magma::inputDomains));










	const Uint8* state = SDL_GetKeyboardState(NULL);
	int modX = 0;
	int modY = 0;
	auto map = Map(256, 256, 1234);
	map.generate_heightmap();
	map.generate_moisturemap();
	map.assign_biomes();
	int rWidth = 80;
	int rHeight = 40;
	rlCommands.activateDomain(magma::INPUT_DOMAIN_DEFAULT);
	rlCommands.activateDomain(magma::INPUT_DOMAIN_TEXTBOX);
	while (true)
	{
		for (int x = 0; x < rWidth; x++)
		{
			for (int y = 0; y < rHeight; y++)
			{
				Biome b;
				if (modX + x < 0 || modY + y < 0 || modX + x > 255 || modY + y > 255)
					b = Biome::OCEAN;
				else 
					b = map.biomemap[modX + x][modY + y];
				switch(b)
				{
				case Biome::OCEAN:
					root->set_fg(70, 17, 219);
					root->putc(x, y, 'O');
					break;
				case Biome::BEACH:
					root->set_fg(229, 244, 66);
					root->putc(x, y, 'B');
					break;
				case Biome::GRASSLAND:
					root->set_fg(17, 219, 27);
					root->putc(x, y, 'G');
					break;
				case Biome::DESERT:
					root->set_fg(229, 244, 66);
					root->putc(x, y, 'D');
					break;
				case Biome::MOUNTAINS:
					root->set_fg(255, 255, 255);
					root->putc(x, y, 'M');
					break;
				}
				
			}
		}
		
		magma::commands cmd;
		std::string name;
		while (rlCommands.getCommand(cmd, name))
		{
			log->info("Command name: {}", name);
		}


		root->refresh();
		root->clear();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				rlCommands.checkCommand(event.type, event);
				break;
			case SDL_QUIT:
				exit(0);
			}
		}

	}
	return 0;
}
