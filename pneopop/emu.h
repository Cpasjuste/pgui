#ifndef __EMU_H__
#define __EMU_H__

#define OPTIONS_ITEMS 7

#define PARAM8_OPTIONS_COUNT 8

#define BOOL 0
#define INT 1
#define CHAR 2

int selected_option_item;
int option_item_y[OPTIONS_ITEMS];

void emu_config_init();
void emu_exec();

typedef struct
{
	char exec[256];
	char param[OPTIONS_ITEMS][256];
	char param_human_readable[OPTIONS_ITEMS][256];
	int param_type[OPTIONS_ITEMS];
	int param_enabled[OPTIONS_ITEMS];
	int param_int_value[OPTIONS_ITEMS];
	char param_char_value[OPTIONS_ITEMS][10][256];
} OPT;

typedef struct
{
	char value[OPTIONS_ITEMS * 2][256];
} ARGS;

OPT *emu;
ARGS *arg;

#endif
