#ifndef __CHUNK_H_
#define __CHUNK_H_

#include "cell.h"

#include "globalvars.h"

inline int abs(int n) {
	return n>0 ? n : -n;
}

class chunk: public cell_grid {
private:
	int x, y;

	static int max_x;

public:
	chunk(int x, int y);

	cell *getCellAt(int cx, int cy);

	bool isChunk(int xx, int yy) {
		return x==xx && y==yy;
	}

	void resetCount();

	void tick();

	bool isEmpty();

	static int row_size() {
		return max_x + 1;
	}

	friend void render(struct SDL_Surface *surface);
	friend void countCells();
};


#endif // __CHUNK_H_
