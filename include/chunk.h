#ifndef __CHUNK_H_
#define __CHUNK_H_

#include "cell.h"

#include "globalvars.h"

class chunk: public cell_grid {
private:
	int x, y;

public:
	chunk(int x, int y);

	cell *getCellAt(int cx, int cy);

	bool isChunk(int xx, int yy) {
		return x==xx && y==yy;
	}

	void resetCount();

	void tick();

	bool isEmpty();

	friend void render(struct SDL_Surface *surface);
	friend void countCells();
};


#endif // __CHUNK_H_
