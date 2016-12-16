#include "cell.h"

void cell::tick() {
	switch (value) {
	case 0x03:
		setAlive(true); break;
	case 0x82:
	case 0x83:
		break;
	default:
		setAlive(false);
	}
}
