#include "file.h"

#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

bool does_file_exist(char* filename)
{
	DWORD dwAttr = GetFileAttributes(filename);
	return (dwAttr != INVALID_FILE_ATTRIBUTES &&
		!(dwAttr & FILE_ATTRIBUTE_DIRECTORY));
}
#elif defined(__linux__)
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
bool does_file_exist(char* filename)
{
	if(access(filename, R_OK | W_OK) != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
#endif
