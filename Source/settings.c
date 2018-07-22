#include "settings.h"

#include <stddef.h>

#include "file.h"
#include "ini.h"

#define DEFAULT_DATA_DIR ./data

typedef struct conf {
	char* data_dir;
} conf;

static conf* settings = NULL;

void read_config()
{
	if (does_file_exist("./config.ini"))
	{
		ini_t* config = ini_load("config.ini");
	}
}
