#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/io.h>

#include "emu.h"
#include "config.h"

void gui_clean();
void gui_load();
void gui_init_sdl();

extern char **roms;

void emu_config_init()
{
	emu = (OPT *) malloc( sizeof(OPT));

	strcpy(emu->exec, "pneopop");

	strcpy(emu->param[0], "-f");
	strcpy(emu->param_human_readable[0], "Frameskip");
	emu->param_type[0] = INT;
	emu->param_int_value[0] = 0;
	emu->param_int_value_max[0] = 7;
	emu->param_enabled[0] = 1;

	strcpy(emu->param[1], "-z");
	strcpy(emu->param_human_readable[1], "Scale");
	emu->param_type[1] = INT;
	emu->param_int_value[1] = 1;
	emu->param_int_value_max[1] = 2;
	emu->param_enabled[1] = 1;

	strcpy(emu->param[2], "-m");
	strcpy(emu->param_human_readable[2], "Smooth");
	emu->param_type[2] = BOOL;
	emu->param_enabled[2] = 0;

	strcpy(emu->param[3], "-S");
	strcpy(emu->param_human_readable[3], "Mute Sound");
	emu->param_type[3] = BOOL;
	emu->param_enabled[3] = 0;

	strcpy(emu->param[4], "-Z");
	strcpy(emu->param_human_readable[4], "Sound Freq");
	emu->param_type[4] = CHAR;
	strcpy(emu->param_char_value[4][0], "11025");
	strcpy(emu->param_char_value[4][1], "22050");
	strcpy(emu->param_char_value[4][2], "44100");
	emu->param_char_value_count[4] = 3;
	emu->param_int_value[4] = 2;
	emu->param_enabled[4] = 1;
}

int build_args()
{
	int i = 0, j = 0;

	for(i = 0; i < OPTIONS_ITEMS ; i++)
	{
		if(emu->param_enabled[i])
		{
			if(emu->param_type[i] == BOOL)
			{
				sprintf(arg->value[j], "%s", emu->param[i]);
				j++;
			}
			else if(emu->param_type[i] == INT)
			{
				sprintf(arg->value[j], "%s", emu->param[i]);
				j++;
				sprintf(arg->value[j], "%i", emu->param_int_value[i] + 1);
				j++;
			}
			else if(emu->param_type[i] == CHAR)
			{
				sprintf(arg->value[j], "%s", emu->param[i]);
				j++;
				sprintf(arg->value[j], "%s", emu->param_char_value[i][emu->param_int_value[i]]);
				j++;
			}
		}
	}
	
	return j;
}

void emu_exec(int n)
{
	pid_t childpid;
	int status;

	gui_clean();

	arg = (ARGS *) malloc( sizeof(ARGS));

	int n_args = build_args();

	char exec_rom[512];
	sprintf( exec_rom, "%s/%s", config->roms_path, roms[n] );

	if ((childpid = fork()) == -1)
	{
		printf("Error in the fork");
	}
	else if (childpid == 0)
	{
		printf("childpid OK\n");

		if(n_args == 0)
		{
			if (execl(emu->exec, emu->exec, exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 1)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 2)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 3)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 4)
		{
			printf("args : %s %s %s %s\n", arg->value[0], arg->value[1], arg->value[2], arg->value[3] );
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 5)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 6)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 7)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 8)
		{
			if (execl(emu->exec, emu->exec,   arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 9)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 10)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 11)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 12)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], arg->value[11], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 13)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], arg->value[11], arg->value[12], exec_rom, NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 14)
		{
			if (execl(emu->exec, emu->exec,  arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], arg->value[11], arg->value[12], arg->value[13], exec_rom, NULL) < 0) printf("Exec failed");
		}
	}
	else if (childpid != wait(&status))
	{ 
		printf("A signal occurred before the child exited");
	}

	free(arg);
	gui_init_sdl();
	gui_load();
}




