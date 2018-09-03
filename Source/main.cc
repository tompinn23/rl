
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
#include "generate/generate.h"
#include "generate/enums.h"

using magma::Console;



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













	const Uint8* state = SDL_GetKeyboardState(NULL);
	int modX = 0;
	int modY = 0;
	auto map = Map(256, 256, 1234);
	map.generate_heightmap();
	map.generate_moisturemap();
	map.assign_biomes();
	int rWidth = 80;
	int rHeight = 40;
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
		
		root->refresh();
		root->clear();

		if (state[SDL_SCANCODE_ESCAPE])
			break;
		if (state[SDL_SCANCODE_DOWN])
			rHeight++;
		if (state[SDL_SCANCODE_RIGHT])
			rWidth++;
		if (state[SDL_SCANCODE_UP])
			rHeight--;
		if (state[SDL_SCANCODE_LEFT])
			rWidth--;
		if (state[SDL_SCANCODE_A])
		{
			modX -= 1;
		}
		if (state[SDL_SCANCODE_D])
		{
			modX += 1;
		}
		if (state[SDL_SCANCODE_W])
		{
			modY -= 1;
		}
		if (state[SDL_SCANCODE_S])
		{
			modY += 1;
		}



		SDL_PumpEvents();

	}
	return 0;
}
