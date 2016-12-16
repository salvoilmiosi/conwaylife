#include "chunk.h"

#include "SDL2/SDL.h"

chunk::chunk(int x, int y) : cell_grid(CHUNK_SIZE, CHUNK_SIZE), x(x),y(y) {}

cell *chunk::getCellAt(int cx, int cy) {
	if (cx < x || cx >= x+w) return NULL;
	if (cy < y || cy >= y+h) return NULL;
	return getItem(cx-x, cy-y);
}

void chunk::resetCount() {
	for (cell *c=data; c-data < w*h; ++c) {
		c->resetCount();
	}
}

void chunk::tick() {
	for (cell *c=data; c-data < w*h; ++c) {
		c->tick();
	}
}

bool chunk::isEmpty() {
	for (cell*c = data; c-data<w*h; ++c) {
		if (! c->isEmpty()) return false;
	}
	return true;
}
