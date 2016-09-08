#ifndef __CONWAY_H_
#define __CONWAY_H_

#include "SDL2/SDL.h"

bool initGrid();
void gridCleanUp();

void tick(const Uint8 *keysHeld);
void step();
void render(SDL_Surface *screen);

#endif // __CONWAY_H_
