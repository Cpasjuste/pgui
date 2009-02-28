#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_framerate.h>
#include <SDL_rotozoom.h>

void gui_clean();

SDL_Surface *load_image(char *filename )
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

 	loadedImage = IMG_Load(filename);

	if( loadedImage != NULL )
	{
		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}
	else
	{
		printf("Unable to load image (%s)\n", filename);
		gui_clean();
		exit(0);
	}

	return optimizedImage;
}

SDL_Surface *load_image_alpha(char *filename )
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load(filename);

	if( loadedImage != NULL )
	{
		if ( loadedImage->format->palette )
			SDL_SetColorKey(loadedImage, SDL_SRCCOLORKEY, *(Uint8 *)loadedImage->pixels);

		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}
	else
	{
		printf("Unable to load image (%s)\n", filename);
		gui_clean();
		exit(0);
	}

	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface( source, NULL, destination, &offset );
}

void apply_surface_center( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	SDL_Rect offset;

	offset.x = (x - (source->w / 2));
	offset.y = (y - (source->h / 2));

	SDL_BlitSurface( source, NULL, destination, &offset );
}

