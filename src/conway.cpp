#include "conway.h"

#include "SDL2/SDL.h"

#include "globalvars.h"

#include "gridio.h"

#include "chunk.h"

#include <map>
#include <iostream>

#include "tinyfiledialogs.h"

struct point {
	int x,y;
	point(int x, int y) : x(x), y(y) {}
	int quadrant() const {
		if (x > 0) {
			return y > 0 ? 0 : 3;
		} else {
			return y > 0 ? 1 : 2;
		}
	}
	int position() const {
		return abs(y) * chunk::row_size() + abs(x);
	}
};

bool operator < (const point & pt1, const point & pt2) {
	// order by quadrants, then by position
	if (pt1.quadrant() == pt2.quadrant()) {
		return pt1.position() < pt2.position();
	} else {
		return pt1.quadrant() < pt2.quadrant();
	}
}

std::map<point, chunk> chunks;

static int xo = 0;
static int yo = 0;

static chunk &getChunkAt(int x, int y) {
	int xx = (x / CHUNK_SIZE) * CHUNK_SIZE; if (x<0) xx -= CHUNK_SIZE -1;
	int yy = (y / CHUNK_SIZE) * CHUNK_SIZE; if (y<0) yy -= CHUNK_SIZE -1;

	try {
		return chunks.at(point(xx,yy));
	} catch (std::out_of_range) {
		return chunks.insert(std::make_pair(point(xx,yy), chunk(xx, yy))).first->second;
	}
}

static cell *getCellAt(int x, int y) {
	cell *c = getChunkAt(x, y).getCellAt(x, y);
	if (!c) throw std::out_of_range(nullptr);
	return c;
}

bool initGrid(const char *filename) {
	gridCleanUp();

	const char *FILTERS[] = {"*.bmp"};
	const char *DESCRIPTIONS = "Bitmaps";
	const char *gridFilename = tinyfd_openFileDialog("Conway", "", 1, FILTERS, DESCRIPTIONS, 0);
	if (!gridFilename) return false;

	cell_grid *g = loadGrid(gridFilename);
	if (g == NULL) {
		char errorMsg[100] = "Error: can't open file\n";
		strncat(errorMsg, gridFilename, sizeof(errorMsg));
		tinyfd_messageBox("Conway", errorMsg, "ok", "error", 1);
		return false;
	}

    cell *c = NULL;
	for (int y=0; y<g->height(); ++y) {
		for (int x=0; x<g->width(); ++x) {
			c = g->getItem(x, y);
			if (c->alive()) getCellAt(x, y)->setAlive(true);
		}
	}

	delete g;

	xo = (SCREEN_W - g->width() * CELL_SIZE) / 2;
	yo = (SCREEN_H - g->height() * CELL_SIZE) / 2;

	return true;
}

void gridCleanUp() {
	chunks.clear();
}

void countCells() {
	for (auto &i : chunks) {
		i.second.resetCount();
	}

	for (auto &i : chunks) {
		chunk &ch = i.second;
		for (int y=0; y<ch.height(); ++y) {
			for (int x=0; x<ch.width(); ++x) {
				cell *c = ch.getItem(x, y);
				if (c->alive()) {
					int ax = x + ch.x;
					int ay = y + ch.y;

					getCellAt(ax-1, ay-1)->addOne();
					getCellAt(ax, ay-1)->addOne();
					getCellAt(ax+1, ay-1)->addOne();
					getCellAt(ax-1, ay)->addOne();
					getCellAt(ax+1, ay)->addOne();
					getCellAt(ax-1, ay+1)->addOne();
					getCellAt(ax, ay+1)->addOne();
					getCellAt(ax+1, ay+1)->addOne();

					// I should really make the GPU do this.
				}
			}
		}
	}
}

void step() {
	countCells();
	auto it = chunks.begin();
	while (it != chunks.end()) {
		chunk &c = it->second;
		if (c.isEmpty()) {
			it = chunks.erase(it);
		} else {
			c.tick();
			++it;
		}
	}
}

void tick(const Uint8 *keysHeld) {
	static const int movespeed = 10;

	if (keysHeld[SDL_SCANCODE_LEFT]) xo -= movespeed;
	if (keysHeld[SDL_SCANCODE_RIGHT]) xo += movespeed;
	if (keysHeld[SDL_SCANCODE_UP]) yo -= movespeed;
	if (keysHeld[SDL_SCANCODE_DOWN]) yo += movespeed;

	if (keysHeld[SDL_SCANCODE_A]) ++CELL_SIZE;
	if (keysHeld[SDL_SCANCODE_Z] && CELL_SIZE > 1) --CELL_SIZE;

	static bool toggleDrawchunks = true;

	if (keysHeld[SDL_SCANCODE_C]) {
		if (toggleDrawchunks) {
			DRAW_CHUNKS = !DRAW_CHUNKS;
			toggleDrawchunks = false;
		}
	} else {
		toggleDrawchunks = true;
	}
}

void render(SDL_Surface *screen) {
	static const Uint32 COLOR_DEAD = SDL_MapRGB(screen->format, 0, 0, 0);
	static const Uint32 COLOR_BG = SDL_MapRGB(screen->format, 0, 0, 255);
	static const Uint32 COLOR_LIVE = SDL_MapRGB(screen->format, 0, 255, 0);

	SDL_FillRect(screen, &screen->clip_rect, DRAW_CHUNKS ? COLOR_BG : COLOR_DEAD);

	for (auto &p : chunks) {
		chunk &ch = p.second;
		if ((ch.x + ch.w) * CELL_SIZE + xo < 0 || ch.x * CELL_SIZE + xo >= SCREEN_W) continue;
		if ((ch.y + ch.h) * CELL_SIZE + yo < 0 || ch.y * CELL_SIZE + yo >= SCREEN_H) continue;

		if (DRAW_CHUNKS) {
			SDL_Rect rct;
			rct.x = (ch.x * CELL_SIZE) + xo;
			if (ch.x < 0) rct.x -= CELL_SIZE;
			rct.y = (ch.y * CELL_SIZE) + yo;
			if (ch.y < 0) rct.y -= CELL_SIZE;
			rct.w = ch.w * CELL_SIZE;
			rct.h = ch.h * CELL_SIZE;
			SDL_FillRect(screen, &rct, COLOR_DEAD);
		}

		int i = 0;
		for (cell *c = ch.data; (i = c-ch.data) < ch.w * ch.h; ++c) {
			if (! c->alive()) continue;

			int x = i % ch.w + ch.x;
			int y = i / ch.w + ch.y;

			SDL_Rect dstrect;
			dstrect.x = CELL_SIZE * x + xo;
			dstrect.y = CELL_SIZE * y + yo;
			dstrect.w = CELL_SIZE;
			dstrect.h = CELL_SIZE;

			SDL_FillRect(screen, &dstrect, COLOR_LIVE);
		}
	}
}
