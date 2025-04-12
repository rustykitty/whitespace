#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifndef NDEBUG
#define DEBUG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG(...)
#endif

#endif