extern "C" {
#include <common/whitespace.h>
#include <common/heap.h>
}

#include <unordered_map>

static std::unordered_map<ws_int, ws_int>* heap = nullptr;

extern "C" {
void heap_init() {
    heap = new std::unordered_map<ws_int, ws_int>();
}

ws_int WS_heap_load(ws_int addr) {
    auto it = heap->find(addr);
    if (it != heap->end()) {
        return it->second;
    }
    return 0;
}

void WS_heap_store(ws_int addr, ws_int val) {
    if (val == 0) {
        heap->erase(addr);
    } else {
        (*heap)[addr] = val;
    }
}

void WS_heap_free() {
    delete heap;
    heap = nullptr;
}
}
