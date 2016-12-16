#ifndef __PIXELACCESS_H_
#define __PIXELACCESS_H_

#include "SDL2/SDL.h"

Uint32 getpixel(SDL_Surface *surface, int x, int y);

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif // __PIXELACCESS_H_
