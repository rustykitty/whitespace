#ifndef WS_OPERATION_H
#define WS_OPERATION_H

typedef long long int ws_int;

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

struct WS_statement {
    enum WS_operation op;
    union {
        ws_int num;
        label_type label;
    };
};

#endif