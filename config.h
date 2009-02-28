#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <libconfig.h>

struct config_t cfg;

typedef struct
{
	char roms_path[512];

} CONFIG;

CONFIG *config;

int cfg_read(char *file);

#endif
