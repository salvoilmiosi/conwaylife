#include "gridio.h"
#include "SDL2/SDL.h"
#include "pixelaccess.h"

void saveGrid(const char *filename, cell_grid *g) {
	SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(0, g->w, g->h, 32,
                                   rmask, gmask, bmask, amask);

	if (surface == NULL) return;

	static const Uint32 white = SDL_MapRGB(surface->format, 0xff, 0xff, 0xff);
	static const Uint32 black = SDL_MapRGB(surface->format, 0x0, 0x0, 0x0);

	cell *c = g->data;
	for (int y=0; y<g->h; ++y) {
		for (int x=0; x<g->w; ++x) {
			putpixel(surface, x,y, c->alive() ? white : black);
			++c;
		}
	}

	SDL_SaveBMP(surface, filename);
}

cell_grid *loadGrid(const char *filename) {
	SDL_Surface *surface = SDL_LoadBMP(filename);
	if (surface == NULL) return NULL;

	cell_grid *g = new cell_grid(surface->w, surface->h);

	static const Uint32 black = SDL_MapRGB(surface->format, 0x0, 0x0, 0x0);

    cell *c;
	for (int y=0; y<surface->h; ++y) {
		for (int x=0; x<surface->w; ++x) {
            c = g->getItem(x, y);
            if (c != NULL) c->setAlive(getpixel(surface, x, y) != black);
		}
	}

	return g;
}
