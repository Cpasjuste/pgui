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

char **roms;

void emu_config_init()
{
	emu = (OPT *) malloc( sizeof(OPT));

	strcpy(emu->exec, "sms_sdl");

	strcpy(emu->param[0], "--fm");
	strcpy(emu->param_human_readable[0], "YM2413 sound");
	emu->param_type[0] = BOOL;
	emu->param_enabled[0] = 0;

	strcpy(emu->param[1], "--japan");
	strcpy(emu->param_human_readable[1], "Domestic machine type" /*as DOMESTIC instead of OVERSEAS"*/);
	emu->param_type[1] = BOOL;
	emu->param_enabled[1] = 0;

	strcpy(emu->param[2], "--usesram");
	strcpy(emu->param_human_readable[2], "Auto load/save SRAM");
	emu->param_type[2] = BOOL;
	emu->param_enabled[2] = 0;

	strcpy(emu->param[3], "--fskip");
	strcpy(emu->param_human_readable[3], "Frameskip");
	emu->param_type[3] = INT;
	emu->param_int_value[3] = 0;
	emu->param_enabled[3] = 0;

	strcpy(emu->param[4], "--fullspeed");
	strcpy(emu->param_human_readable[4], "Fullspeed");
	emu->param_type[4] = BOOL;
	emu->param_enabled[4] = 0;

	strcpy(emu->param[5], "--nosound");
	strcpy(emu->param_human_readable[5], "Mute sound");
	emu->param_type[5] = BOOL;
	emu->param_enabled[5] = 0;

	strcpy(emu->param[6], "--filter");
	strcpy(emu->param_human_readable[6], "Filter");
	emu->param_type[6] = CHAR;
	strcpy(emu->param_char_value[6][0], "2xsai");
	strcpy(emu->param_char_value[6][1], "super2xsai");
	strcpy(emu->param_char_value[6][2], "supereagle");
	strcpy(emu->param_char_value[6][3], "advmame2x");
	strcpy(emu->param_char_value[6][4], "tv2x");
	strcpy(emu->param_char_value[6][5], "2x");
	strcpy(emu->param_char_value[6][6], "bilinear");
	strcpy(emu->param_char_value[6][7], "dotmatrix");
	emu->param_int_value[6] = 4;
	emu->param_enabled[6] = 1;

}

int build_args()
{
	arg = (ARGS *) malloc( sizeof(ARGS));

	int i = 0, j = 0;

	for(i = 0; i < OPTIONS_ITEMS; i++)
	{
		if(emu->param_enabled[i])
		{
			if(emu->param_type[i] == BOOL)
			{
				strcpy(arg->value[j], emu->param[i]);
				j++;
			}
			else if(emu->param_type[i] == INT)
			{
				strcpy(arg->value[j], emu->param[i]);
				j++;
				sprintf(arg->value[j], "%i", emu->param_int_value[i]);
				j++;
			}
			else if(emu->param_type[i] == CHAR)
			{
				strcpy(arg->value[j], emu->param[i]);
				j++;
				strcpy(arg->value[j], emu->param_char_value[i][emu->param_int_value[i]]);
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

	char exec_rom[512];
	sprintf( exec_rom, "%s/%s", config->roms_path, roms[n] );

	printf("Selected rom = %s\n", exec_rom);

	int n_args = build_args();

	if ((childpid = fork()) == -1)
	{
		printf("Error in the fork");
	}
	else if (childpid == 0)
	{
		printf("childpid OK\n");

		if(n_args == 0)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 1)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 2)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 3)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 4)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 5)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 6)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 7)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 8)
		{
			if (execl(emu->exec, emu->exec,  exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 9)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 10)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 11)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 12)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], arg->value[11], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 13)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], arg->value[11], arg->value[12], NULL) < 0) printf("Exec failed");
		}
		else if(n_args == 14)
		{
			if (execl(emu->exec, emu->exec, exec_rom, "--joystick", arg->value[0], arg->value[1], arg->value[2], arg->value[3], arg->value[4], arg->value[5], arg->value[6], arg->value[7], arg->value[8], arg->value[9], arg->value[10], arg->value[11], arg->value[12], arg->value[13], NULL) < 0) printf("Exec failed");
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




