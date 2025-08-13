#ifndef HEAP_H
#define HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../whitespace.h"

#include <stdbool.h>
#include <sys/types.h>

void WS_heap_init();
ws_int WS_heap_load(ws_int);
void WS_heap_store(ws_int, ws_int);
void WS_heap_free();

#ifdef __cplusplus
}
#endif

#endif // HEAP_H