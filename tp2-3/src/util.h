#ifndef __UTIL_H
#define __UTIL_H

#include "extern.h"

Object *parse(char *filename);

void display_object(Object obj);
void display_objects(Object *objects);

#endif
