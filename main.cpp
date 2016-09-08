#include "SDL2/SDL.h"

#include "globalvars.h"

#include "conway.h"

static SDL_Surface *screen;
static SDL_Window *window;

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	window = SDL_CreateWindow("Conway's Game of Life",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	screen = SDL_GetWindowSurface(window);
	if (screen == NULL)
		return false;

	return true;
}

inline void cleanUp() {
	gridCleanUp();
	SDL_Quit();
}

int main(int argc, char **argv) {
    readOptions();

    if (CHUNK_SIZE <= 0) CHUNK_SIZE = 1;
    if (FPS < 0) FPS = 0;

    if (!initGrid()) return 2;
    if (!init()) return 1;

	bool quit = false;
	SDL_Event event;
	int ticks = 0;

	int ts;

    while (!quit) {
		if (FRAME_SKIP <= 0 || ticks++ % FRAME_SKIP == 0) {
			render(screen);
			SDL_UpdateWindowSurface(window);
		}

		const Uint8 *keysHeld = SDL_GetKeyboardState(NULL);

		for (ts = TICK_SKIP; ts>=0; --ts) {
            step();
		}

        tick(keysHeld);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					SCREEN_W = event.window.data1;
					SCREEN_H = event.window.data2;

					screen = SDL_GetWindowSurface(window);
				}
				break;
			}
		}

		if (FPS > 0) SDL_Delay(1000 / FPS);
    }

	cleanUp();
	saveOptions();
    return 0;
}

