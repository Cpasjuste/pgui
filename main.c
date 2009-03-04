#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/io.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_framerate.h>
#include <SDL_rotozoom.h>

#include "main.h"
#include "graphics.h"
#include "mouse.h"
#include "config.h"
#include "emu.h"

static struct dirent *my_readdir (DIR *dirstream, struct dirent *space)
{
	struct dirent *loc;
	if (readdir_r (dirstream, space, &loc) == 0)
	{
		/* Success */
		return loc;
	}
	return 0;
}

void filer_read_dir(char *directory)
{
	struct dirent *fd;

	DIR * rep = opendir (directory);

	memset(&list, 0, sizeof(struct dirent));

	int i;
	for (i = 0; i < list_num; i++)
	{
        	free(roms[i]);
	}
	free(roms);

	list_num = 0;

	while((fd = my_readdir(rep, &list[list_num])) && (list_num<MAXDIRNUM)) 
	{
		if (list[list_num].d_name[0] == '.') continue;
		else if (list[list_num].d_type == DT_DIR) continue;

		if(fd != NULL) list_num++;
	}
	closedir(rep);

	if (list_start  >= list_num) { list_start  = list_num-1; }
	if (list_start  <  0)         { list_start  = 0;           }
	if (list_curpos >= list_num) { list_curpos = list_num-1; }
	if (list_curpos <  0)         { list_curpos = 0;           }

	if(list_num < 1) noDir=1; else noDir=0;
	
	roms = (char**) malloc(list_num * sizeof(char*));

	for (i = 0; i < list_num; i++)
	{
		roms[i] = (char*) malloc(256);
		strcpy(roms[i], list[i].d_name);
	}
	sort();
}

int gui_init_sdl()
{
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) == -1 )
	{
		fprintf(stderr, "Couldn’t initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	int joystick = 0, x = 0;
	joystick = SDL_NumJoysticks();
	printf("Joysticks spotted : %d\n", joystick);

	for (x = 0; x < joystick; x++)
	{
		SDL_Joystick * joy = SDL_JoystickOpen(x);
		printf("Joystick %i: %s\n", x, SDL_JoystickName(x));
		printf("Axes: %d\n", SDL_JoystickNumAxes(joy));
		printf("Boutons: %d\n", SDL_JoystickNumButtons(joy));
		printf("Trackballs: %d\n", SDL_JoystickNumBalls(joy));
		printf("Hats: %d\n\n", SDL_JoystickNumHats(joy));
	}

	myscreen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if( myscreen == NULL )
	{
		fprintf(stderr, "Couldn’t initialize VideoMode: %s\n", SDL_GetError());
		return 0;
	}

	if( TTF_Init() == -1 )
	{ 
		printf("TTF_Init() failed : %s\n", SDL_GetError());
		return 0;
	} 

	font = TTF_OpenFont( "data/font.ttf", 16); 
	if( font == NULL )
	{
		printf("TTF_OpenFont failed : %s\n", SDL_GetError());
		return 0; 
	} 

	font_big = TTF_OpenFont( "data/font.ttf", 20); 
	if( font == NULL )
	{
		printf("TTF_OpenFont failed : %s\n", SDL_GetError());
		return 0; 
	} 
	
#ifndef PANDORA
	SDL_ShowCursor(SDL_ENABLE);
#else
	SDL_ShowCursor(SDL_DISABLE);
#endif

	return 1;
}

void gui_load()
{
	text_menu[0] = TTF_RenderUTF8_Blended( font, "Roms", WHITE );
	text_menu[1] = TTF_RenderUTF8_Blended( font, "Options", WHITE );
	text_menu[2] = TTF_RenderUTF8_Blended( font, "Quit", WHITE );

	background = load_image( "data/backg.bmp" );
	highlight = load_image_alpha( "data/highlight.bmp" );

	confirm_box = load_image_alpha("data/confirm_box.bmp");
}

void gui_clean()
{
	if( text_menu[0] != NULL) SDL_FreeSurface( text_menu[0] );
	if( text_menu[1] != NULL) SDL_FreeSurface( text_menu[1] );
	if( text_menu[2] != NULL) SDL_FreeSurface( text_menu[2] );

	if( highlight != NULL) SDL_FreeSurface( highlight );
	if( confirm_box != NULL) SDL_FreeSurface( confirm_box );
	if( background != NULL) SDL_FreeSurface( background );
	TTF_CloseFont( font ); 
	TTF_CloseFont( font_big );
	TTF_Quit(); 
	SDL_Quit();
}

void gui_scroll_text(int y, char *text)
{
	int i = strlen(text) * 12;

	if(scroll_count < -i) scroll_count = 800;

	message = TTF_RenderUTF8_Solid( font, text, WHITE );
	SDL_Surface *new_message = SDL_DisplayFormat( message );
	SDL_FreeSurface( message );

	apply_surface( scroll_count, y, new_message, myscreen );
	SDL_FreeSurface( new_message );

	scroll_count -= 1;
}

void gui_draw()
{
	apply_surface( 0, 0, background, myscreen );

	if(selected_menu == ROMS)
	{
		int i = list_start;
		int j = 0;

		while (i<(list_start+MAXLIST)) 
		{
			rom_text_x[j] = 0;
			rom_text_y[j] = 0;
			rom_text_w[j] = 0;
			rom_text_h[j] = 0;

			if (i<list_num)
			{
				strncpy(path_tmp, roms[i], MAXPATH);

				if (i == list_curpos)
				{
					message = TTF_RenderUTF8_Blended( font_big, path_tmp, GREEN );
				}
				else
				{
					message = TTF_RenderUTF8_Blended( font, path_tmp, WHITE );
				}
				apply_surface( 60, ((i - list_start) + 4 ) * (MAXLIST+15), message, myscreen );

				rom_text_x[j] = 60;
				rom_text_y[j] = ((i - list_start) + 4 ) * (MAXLIST+15);
				rom_text_w[j] = message->w;
				rom_text_h[j] = message->h;

				SDL_FreeSurface( message );
			}
			i++; j++;
		}		
	}
	else if(selected_menu == OPTIONS)
	{
		int i, y = 120;

		for( i = 0; i < OPTIONS_ITEMS; i++ )
		{
			char tmpMessage[512];

			if(emu->param_type[i] == BOOL)
			{
				if(emu->param_enabled[i]) sprintf(tmpMessage, "%s : True", emu->param_human_readable[i]);
					else sprintf(tmpMessage, "%s : False", emu->param_human_readable[i]);
		
			}
			else if(emu->param_type[i] == INT)
			{
				sprintf(tmpMessage, "%s : %i", 	emu->param_human_readable[i], emu->param_int_value[i]);				
			}
			else if(emu->param_type[i] == CHAR)
			{
				sprintf(tmpMessage, "%s : %s", 	emu->param_human_readable[i], emu->param_char_value[i][emu->param_int_value[i]]);
			}
		
			if(selected_option_item == i) message = TTF_RenderUTF8_Blended( font_big, tmpMessage, GREEN );
				else message = TTF_RenderUTF8_Blended( font, tmpMessage, WHITE );

			apply_surface( 60, y, message, myscreen );
			SDL_FreeSurface( message );
	
			option_item_y[i] = y;
			
			y += 45;
		}
	}

	if( alpha < 172 && alpha_up == 1)
	{
		alpha += 4;
		if(alpha == 168) alpha_up = 0;
			else alpha_up = 1;
	}
	else if( alpha > 100 && alpha_up != 1)
	{
		alpha -= 4;
		if(alpha == 100) alpha_up = 1;
			else alpha_up = 0;
	}
	SDL_SetAlpha( highlight, SDL_SRCALPHA | SDL_RLEACCEL, alpha );
	apply_surface_center( text_menu_x[selected_menu], text_menu_y[selected_menu], highlight, myscreen );

	int i;
	for( i = 0; i < MENU_COUNT; i++)
	{
		apply_surface_center( text_menu_x[i], text_menu_y[i], text_menu[i], myscreen);
	}

	//gui_scroll_text(455, applications[selected_menu]->description[list_curpos[selected_menu]]);
}

int gui_confirm_box(char *msg)
{
	int done = 0;

	gui_draw();
	apply_surface_center(400, 240, confirm_box, myscreen);
	message = TTF_RenderUTF8_Blended( font, msg, WHITE );
	apply_surface_center(400, 240, message, myscreen);
	SDL_FreeSurface( message );
	SDL_Flip(myscreen);

	while(!done)
	{
		get_mouse_loc();

		if(get_mouse_click(MOUSE_LEFT))
		{
			if(reset_ts_pos) { mouse_hold_x = MOUSE_X; mouse_hold_y = MOUSE_Y; reset_ts_pos = 0; }

			if(mouse_is_over(300, 378, 285, 322)) // OK
			{
				return 1;
			}
			else if(mouse_is_over(428, 508, 285, 322)) // Cancel
			{
				return 0;
			}
		}
		else reset_ts_pos = 1;

		SDL_framerateDelay( &sixteen );
	}
	return 0;
}

void handle_mouse()
{
	get_mouse_loc();

	if(get_mouse_click(MOUSE_LEFT))
	{	
		if(reset_ts_pos) { mouse_hold_x = MOUSE_X; mouse_hold_y = MOUSE_Y; reset_ts_pos = 0; }

		//printf("X: %i | Y: %i\n", MOUSE_X, MOUSE_Y);

		int i;
		for(i = 0; i < MENU_COUNT; i++ )
		{
			if(mouse_is_over_surface_center(text_menu_x[i], text_menu_y[i], text_menu[i]->w, text_menu[i]->h))
			{
				if(i == EXIT)
				{
					if(gui_confirm_box("Confirm Exit ?"))
					{
						gui_clean();
						exit(0);
					}
				}
				else
				{
					selected_menu = i;
					alpha = 0;
					alpha_up = 1;
					scroll_count = 800;
				}
			}
		}

		if(selected_menu == OPTIONS)
		{
			for(i = 0; i < OPTIONS_ITEMS; i++ )
			{
				if( mouse_is_over(60, 390, option_item_y[i], option_item_y[i] + 25 ))
				{
					if( i == selected_option_item)
					{
						if(emu->param_type[i] == BOOL)
						{
							if(emu->param_enabled[i]) emu->param_enabled[i] = 0;
								else emu->param_enabled[i] = 1;
		
						}
						else if(emu->param_type[i] == INT)
						{
							if(emu->param_int_value[i] < emu->param_int_value_max[i]) emu->param_int_value[i]++;
								else  emu->param_int_value[i] = 0;	
						}
						else if(emu->param_type[i] == CHAR)
						{
							if(emu->param_int_value[i] < emu->param_char_value_count[i]) emu->param_int_value[i]++;
								else emu->param_int_value[i] = 0;
						}
						SDL_Delay(120);
						break;
					}
					else
					{
						selected_option_item = i;
						SDL_Delay(120);
						break; 
					}
				}				
			}
		}

		else if(selected_menu == ROMS)
		{
			for(i = 0; i < MAXLIST; i++ )
			{
				if(rom_text_x[i] > 0)
				{
					if( mouse_is_over_surface(rom_text_x[i], rom_text_y[i], rom_text_w[i], rom_text_h[i] ))
					{
						if(list_curpos == i + (page * MAXLIST))
						{
							app_number = (i + (page * MAXLIST));
							exec_app = 1;
							SDL_Delay(120);
							scroll_count = 800;
							break;
						}
						else
						{
							list_curpos = i + (page * MAXLIST) ;
							SDL_Delay(120);
							scroll_count = 800;
							break; 
						}
					}
				}
			}
/*
			if(mouse_is_over(96, 380, 100, 440))
			{
				if(MOUSE_Y > (mouse_hold_y + 40))
				{
					if (list_curpos > 0) 
					{
						list_curpos--;
						if (list_curpos < list_start)
							list_start = list_curpos;

						mouse_hold_y += 40;
					}			
				}
				else if(MOUSE_Y < (mouse_hold_y - 40))
				{
					if (list_curpos < (list_num-1)) 
					{
						list_curpos++;
						if (list_curpos >= (list_start+MAXLIST))
							list_start++;

						mouse_hold_y -= 40;
					}					
				}
				page = list_curpos / MAXLIST;
				scroll_count = 800;
				SDL_Delay(60);
			}
*/	
			if(mouse_is_over(355, 385, 115, 145))
			{
				if (list_curpos > MAXLIST-1)
				{
					page -= 1;
					list_curpos = page * MAXLIST;
					list_start = page * MAXLIST;
				}
				else
				{
					list_curpos = 0;
					list_start = 0;
				}
				scroll_count = 800;
				SDL_Delay(120);					
			}
			else if(mouse_is_over(355, 385, 400, 425))
			{
				if (list_curpos < ( list_num - MAXLIST ) )
				{
					page += 1;
					list_curpos = page * MAXLIST;
					list_start = page * MAXLIST;
				}
				else
				{
					list_curpos = list_num-1;		
				}
				scroll_count = 800;
				SDL_Delay(120);		
			}
/*
			else if(mouse_is_over(335, 365, 205, 230))
			{
				if (list_curpos > 0) 
				{
					list_curpos--;
					if (list_curpos < list_start)
						list_start = list_curpos;			
				}
				page = list_curpos / MAXLIST;
				scroll_count = 800;
				SDL_Delay(60);			
			}
			else if(mouse_is_over(335, 365, 325, 345))
			{
				if (list_curpos < (list_num-1)) 
				{
					list_curpos++;
					if (list_curpos >= (list_start+MAXLIST))
						list_start++;	
				}
				page = list_curpos / MAXLIST;
				scroll_count = 800;
				SDL_Delay(60);		
			}
*/
		
		}
	}
	else
	{
		if(exec_app)
		{
			if(gui_confirm_box("Launch Rom ?"))
			{
				emu_exec(app_number); exec_app = 0;
			}
			else exec_app = 0;
		}
		reset_ts_pos = 1;
	}
}

int main(int argc, char *argv[])
{

	int gui_done = 0;
	scroll_count = 800;
	selected_menu = ROMS;
	alpha_up = 1;
	alpha = 0;
	mouse_y_pos = 120;
	selected_option_item = 0;

	if(!cfg_read("config.txt"))
	{
		printf("Fail => cfg_read()\n");
		exit(0);
	}

	if(!gui_init_sdl())
	{
		printf("Fail => gui_init_sdl()\n");
		exit(0);
	}

	emu_config_init();
	
	gui_load();

	strncpy(now_path, config->roms_path, MAXPATH);
	filer_read_dir(now_path);

	SDL_initFramerate( &sixteen );
	SDL_setFramerate( &sixteen, 65 );

	while(!gui_done)
	{
		handle_mouse();

		while( SDL_PollEvent( &event ) )
		{	
			switch(event.type)
			{
				case SDL_QUIT:
				{
					gui_clean();
					free(emu);
					exit(0);
				}
				break;
			}
		}
		gui_draw();
		SDL_Flip(myscreen);
		SDL_framerateDelay( &sixteen );
	}
	return 0;
}

