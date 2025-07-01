#include "heap.h"

#include <unordered_map>

static std::unordered_map<ws_int, ws_int> heap;

extern "C" {
    ws_int WS_heap_load(ws_int addr) {
        auto it = heap.find(addr);
        if (it != heap.end()) {
            return it->second;
        }
        return 0;
    }

    void WS_heap_store(ws_int addr, ws_int val) {
        heap[addr] = val;
    }
}
