#ifndef __CREATEGRID_H_
#define __CREATEGRID_H_

#include "cell.h"

void saveGrid(const char *filename, cell_grid *g);

cell_grid *loadGrid(const char *filename);

#endif // __CREATEGRID_H_
