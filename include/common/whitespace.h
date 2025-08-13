#ifndef WHITESPACE_H
#define WHITESPACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>

typedef intmax_t ws_int;

enum WS_operation {
    WS_NONE = 0,
    WS_PUSH = 1,
    WS_DUP = 2,
    WS_COPY = 3,
    WS_SWAP = 4,
    WS_POP = 5,
    WS_SLIDE = 6,
    WS_ADD = 7,
    WS_SUB = 8,
    WS_MUL = 9,
    WS_DIV = 10,
    WS_MOD = 11,
    WS_STORE = 12,
    WS_LOAD = 13,
    WS_LABEL = 14,
    WS_CALL = 15,
    WS_JMP = 16,
    WS_JZ = 17,
    WS_JLZ = 18,
    WS_RET = 19,
    WS_END = 20,
    WS_OUTCHR = 21,
    WS_OUTNUM = 22,
    WS_INCHR = 23,
    WS_INNUM = 24,
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

#ifdef __cplusplus
}
#endif

#endif // WHITESPACE_H
