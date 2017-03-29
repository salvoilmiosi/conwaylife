#include "chunk.h"

#include "SDL2/SDL.h"

int chunk::max_x = 100;

chunk::chunk(int x, int y) : cell_grid(CHUNK_SIZE, CHUNK_SIZE), x(x),y(y) {
	if (abs(x) > max_x) max_x = abs(x);
}

chunk::chunk(const chunk &c) : cell_grid(c) {
	x = c.x;
	y = c.y;
}

chunk::chunk(chunk &&c) : cell_grid(c) {
	x = c.x;
	y = c.y;
}

cell *chunk::getCellAt(int cx, int cy) {
	if (cx < x || cx >= x+w) return nullptr;
	if (cy < y || cy >= y+h) return nullptr;
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