#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>

#include "config.h"

int cfg_read(char *file)
{
	config_init(&cfg);

	printf("loading [%s]..\n", file);

	if (!config_read_file(&cfg, file))
	{
		printf("config_read_file() failed\n");
		return -1;
	}
	else
	{
		config = (CONFIG *) malloc( sizeof(CONFIG));

		config_setting_t *search = NULL;
		search = config_lookup(&cfg, "general");	

		if (!search)
		{
			printf("config_lookup() failed\n");
			return -1;
		}
		else
		{
			config_setting_t *tmp = config_setting_get_member(search, "roms_path");
			if(tmp)
			{ 
				strcpy(config->roms_path, config_setting_get_string(tmp));
				printf("roms_path = %s\n", config->roms_path);

			}
			else
			{
				printf("config_lookup() failed\n");
				return -1;
			}	
		}
	}
	config_destroy(&cfg);

	return 1;
}

