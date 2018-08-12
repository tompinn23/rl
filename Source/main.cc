
#include <iostream>
#include <chrono>
#include <bitset>
//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "spdlog/spdlog.h"

#include "platform.h"

#include "magma/Console.h"

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


int main(int argc, char* argv[])
{
	init_logging();
	auto log = spdlog::get("main");
	log->info("Creating Screen.");
	magma::init();
	auto root = Console::init_root(80, 40, "Taffer_10x10.png", 10);
	auto con = Console::init(40, 20);
	while (true)
	{
		root->putc(0, 0, 'a');
		root->print(2, 5, "Hello");
		root->set_bg(20, 214, 50);
		root->print(5, 3, "LOL XD");

		root->set_fg(20, 214, 50);
		root->set_bg(123, 44, 124);
		root->print(5, 8, "LOL XD");
		root->set_fg(255, 255, 255);
		root->set_bg(0, 0, 0);


		con->set_bg(20, 30, 40);
		con->print(0, 0, "Hello its the console your looking for.");
		con->refresh();
		con->set_bg(0, 0, 0);
		root->blit(con, 0, 0, 40, 20, 10, 10);
		
		root->refresh();
		root->clear();
	}
	return 0;
}
