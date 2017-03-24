#ifndef __GRID_H_
#define __GRID_H_

#include <cstring>

template<typename T>
class grid {
protected:
	T *data;
	int w, h;

public:
	grid(int w, int h) : w(w), h(h) {
        data = new T[w*h];
	}

	grid(const grid &g) {
		w = g.w;
		h = g.h;
		data = new T[w*h];
		memcpy(data, g.data, w*h*sizeof(T));
	}

	grid (grid &&g) {
		w = g.w;
		h = g.h;
		data = g.data;
		g.data = nullptr;
	}

	virtual ~grid() {
		if (data)
			delete[] data;
	}

	T *getItem(int x, int y) {
        if (x < 0 || x >= w) return NULL;
        if (y < 0 || y >= h) return NULL;

        return data + (y*w + x);
    }


	int width() {
		return w;
	}

	int height() {
		return h;
	}
};


#endif // __GRID_H_
