#ifndef __CELL_H_
#define __CELL_H_

#include "grid.h"

class cell {
private:
	unsigned char value;

public:
	cell() {
		value = 0;
	}

public:
	bool alive() {
		return (value & 0x80) != 0;
	}

	void setAlive(bool live) {
		if (live) value |= 0x80;
		else value &= 0x7f;
	}

	unsigned char getCount() {
		return value & 0xf;
	}

	void resetCount() {
		value &= 0x80;
	}

	void addOne() {
		++value;
	}

	bool isEmpty() {
		return value == 0;
	}

	void tick();
};

class cell_grid: public grid<cell> {
public:
	cell_grid(int w, int h) : grid<cell>(w, h) {}

	friend void saveGrid(const char *filename, cell_grid *g);
	friend cell_grid *loadGrid(const char *filename);
};

#endif // __CELL_H_
