#ifndef __GLOBALVARS_H
#define __GLOBALVARS_H

#include <string>

#define GLOBAL_VARS				\
VAR(int, SCREEN_W, 800)			\
VAR(int, SCREEN_H, 600)			\
VAR(int, CELL_SIZE, 1)			\
VAR(int, FPS, 30)				\
VAR(int, FRAME_SKIP, 0)			\
VAR(int, TICK_SKIP, 0)          \
VAR(int, CHUNK_SIZE, 20)		\
VAR(bool, DRAW_CHUNKS, 0)

#define VAR(type, name, default) extern type name;

GLOBAL_VARS

const char *const FILENAME = "options.txt";
const char *const FIRST_LINE = "Game of Life: Options file";

void readOptions(const char *path);
void saveOptions(const char *path);

#endif // __GLOBALVARS_H

