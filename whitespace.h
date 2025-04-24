#ifndef WHITESPACE_H
#define WHITESPACE_H

#include <stdint.h>

typedef intmax_t ws_int;

enum WS_operation {
    WS_PUSH = 0,
    WS_DUP = 1,
    WS_COPY = 2,
    WS_SWAP = 3,
    WS_POP = 4,
    WS_SLIDE = 5,
    WS_ADD = 6,
    WS_SUB = 7,
    WS_MUL = 8,
    WS_DIV = 9,
    WS_MOD = 10,
    WS_STORE = 11,
    WS_LOAD = 12,
    WS_LABEL = 13,
    WS_CALL = 14,
    WS_JMP = 15,
    WS_JZ = 16,
    WS_JLZ = 17,
    WS_RET = 18,
    WS_END = 19,
    WS_OUTCHR = 20,
    WS_OUTNUM = 21,
    WS_INCHR = 22,
    WS_INNUM = 23,
};

typedef const char* label_type;

/**
 * Information on the union:
 * - `num` is undefined when `op` is not a valid operation that takes an 
 *   integral argument.
 * - `label` is defined when `op` is a valid operation which takes a label
 *   argument, but only before label precomputation.
 * - `label_ptr` is defined when `op` is a valid operation which takes a 
 *   label argument, but after label precomputation.
 */
struct WS_statement {
    enum WS_operation op;
    union {
        ws_int num;
        label_type label;
        struct WS_statement* label_ptr;
    };
};

#endif