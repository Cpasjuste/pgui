#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

SDL_Surface *load_image(char *filename );
SDL_Surface *load_image_alpha(char *filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void apply_surface_center( int x, int y, SDL_Surface* source, SDL_Surface* destination );

#endif
