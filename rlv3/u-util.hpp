#ifndef U_UTIL_H
#define U_UTIL_H

#include "top-level.hpp"

#include <string>
#include <vector>

bool should_quit();
void init_stuff();
void init_logging();
void init_signals();
void quit(std::string msg, int error_code);


#endif