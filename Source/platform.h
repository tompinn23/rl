#pragma once

#if defined(_WIN32)
#define Windows // Windows
#elif defined(_WIN64)
#define Windows // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define Windows // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__linux__)
#define Linux // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || defined(__APPLE__) && defined(__MACH__)
#include <sys/param.h>
#if defined(BSD)
#define BSD // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
#endif
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
#define Mac OSX // Apple OSX
#else
#define PLATFORM_NAME NULL
#endif