
#include <iostream>
#include <chrono>
#include <bitset>
//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "lodepng.h"
#include "spdlog/spdlog.h"

#include "platform.h"
#include "z-graphics.h"


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


int main(int argc, char* argv[])
{
	atexit(SDL_Quit);
	init_logging();
	auto log = spdlog::get("main");
	log->info("Creating Screen.");

	int64_t a = (1 << 31) - 1;
	int64_t b = (1 << 63) & !a;
	std::bitset<64> x(a);
	std::cout << x << "\n";
	std::bitset<64> y(b);
	std::cout << y << "\n";

	Screen screen = Screen(40, 40, 10, 10, "Taffer_10x10.png");	
	screen.init();
	screen.set_fg(25, 200, 130);
	screen.set_bg(255, 0, 0);
	for (int i = 0; i < 40; i++)
	{
		screen.draw_char(i, 4, i);
	}
	for (int i = 0; i < 40; i++)
	{
		screen.draw_char(i, 5, i + 40);
	}
	screen.set_fg(0, 0, 0);
	screen.set_bg(255, 255, 255);
	screen.print(2, 2, "Helps Me!");
	for(int i = 0; i < 200; i++)
	{
		screen.set_fg(rand() % 255, rand() % 255, rand() % 255);
		screen.set_bg(rand() % 255, rand() % 255, rand() % 255);
		for(int x = 0; x < 40; x++)
		for(int y = 0; y < 40; y++)
		{
			screen.draw_char(x , y, rand() % 255);
		}
		screen.redraw();
	}
	screen.redraw();
	bool quit = false;
	SDL_Event event;
	while(!quit)
	{
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_DOWN:
						screen.set_fg(rand() % 255, rand() % 255, rand() % 255);
						screen.set_bg(rand() % 255, rand() % 255, rand() % 255);
						for(int x = 0; x < 40; x++)
						for(int y = 0; y < 40; y++)
						{
							screen.draw_char(x , y, rand() % 255);
						}
						screen.redraw();
						break;
				}
				break;
		}

	}

	return 0;
}
