#ifndef __MAIN_H__
#define __MAIN_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_framerate.h>

#define MENU_COUNT 3
#define ROMS 0
#define OPTIONS 1
#define EXIT 2

#define MAXLIST 12
#define MAXDIRNUM 1024
#define MAXPATH 64

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

#define B1 1
#define B2 2
#define L 4
#define R 5
#define	SELECT 8
#define START 9

#define UP 0
#define DOWN 1

struct dirent 		list[MAXDIRNUM];
int			list_num;
char			now_path[512];
char			target[MAXPATH];
char			path_tmp[MAXPATH];
int			list_start;
int			list_curpos;
int			cbuf_start[MAXLIST];
int			cbuf_curpos[MAXLIST];
int			now_depth;
int noDir;

int rom_text_x[MAXLIST], rom_text_y[MAXLIST], rom_text_w[MAXLIST], rom_text_h[MAXLIST];

static int selected_menu = 0, mouse_hold_x = 0, mouse_hold_y = 0, reset_ts_pos = 1, exec_app = 0, \
	mouse_y_pos = 0, page = 0, scroll_count = 0, alpha = 0, alpha_up = 0, app_number = 0;

int sort();
char **roms;


const int text_menu_x[3] = { 80, 170, 260 };
const int text_menu_y[3] = { 42, 42, 42 };
SDL_Surface *text_menu[3];

//static int preview_x[256];
//static int preview_y[256];

TTF_Font *font;
TTF_Font *font_big;

SDL_Event event;

FPSmanager sixteen;

SDL_Surface *message = NULL, *background = NULL, *myscreen = NULL, *highlight = NULL, *confirm_box = NULL;
SDL_Surface *preview[256];

SDL_Color WHITE = { 255, 255, 255 };
SDL_Color BLUE = { 0, 0, 255 };
SDL_Color RED = { 255, 0, 0 };
SDL_Color GREEN = { 0, 255, 0 };
SDL_Color GRAY = { 175, 175, 175 };

#endif
