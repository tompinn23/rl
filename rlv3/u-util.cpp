#include "u-util.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <new>
#include "BearLibTerminal.h"

#include "spdlog/spdlog.h"

static bool quitPlease = false;


void no_memory()
{
	quit("Out of memory!", EXIT_FAILURE);
}

bool should_quit()
{
	return quitPlease;
}

void init_stuff()
{
	init_logging();
	spdlog::get("main")->info("Loggging Initialized.");
	std::set_new_handler(no_memory);
	spdlog::get("main")->info("New Handler Initialized.");
	init_signals();
}


#if defined(Windows)

#include <Windows.h>

BOOL WINAPI consoleHandler(_In_ DWORD signal)
{
	switch (signal)
	{
	case CTRL_C_EVENT:
		spdlog::get("main")->info("CTRL-C received quitting.");
		quitPlease = true;
		return TRUE;
	default:
		return FALSE;
	}
}

void init_signals()
{
	SetConsoleCtrlHandler(consoleHandler, TRUE);
}
#elif defined(Linux)
//TODO: Add linux stuff.
void init_signals()
{

}
#endif


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
		sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("rlv3.log", ));
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
		quit("Log initialization failed: " + std::string(ex.what()), EXIT_FAILURE);
	}
}


void quit(std::string msg, int exit_code)
{
	std::cerr << msg << "\n";
	exit(exit_code);
}