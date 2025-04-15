#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>

#include "ws_operation.h"
#include "debug.h"

#ifndef STACK_SIZE
#define STACK_SIZE 512
#endif

#ifndef CALLSTACK_SIZE
#define CALLSTACK_SIZE 128
#endif

#if STACK_SIZE < 2
#error "STACK_SIZE cannot be less than 2."
#endif

#if CALLSTACK_SIZE < 2
#error "CALLSTACK_SIZE cannot be less than 2."
#endif

struct wstree_err {
    size_t index;
    const char* message;
};

struct heap_entry {
    size_t address;
    ws_int value;
};

static struct heap_entry* heap = NULL;
static size_t heap_size = 0;
static size_t heap_capacity = 0;

static char err_buf[127];

#define GET_ERROR_STRUCT(_i, _m) ({          \
    strcpy(err_buf, _m);\
    struct wstree_err* _err = malloc(sizeof(struct wstree_err)); \
    *_err = (struct wstree_err){             \
        _i, err_buf                          \
    };                                       \
    _err;                                    \
})

#define GET_FORMATTED_ERROR_STRUCT(i, ...) ({ \
    snprintf(err_buf, sizeof(err_buf), __VA_ARGS__); \
    GET_ERROR_STRUCT(i, err_buf); \
})

static struct heap_entry* _get_heap_node(size_t addr) {
    for (size_t i = 0; i < heap_size; ++i) {
        if (heap[i].address == addr) {
            return &heap[i];
        }
    }
    return NULL;
}

static struct wstree_err* e = NULL;

static void heap_store(ws_int addr, ws_int val) {
    if (!heap) {
        heap = malloc(sizeof(struct heap_entry));
        heap_size = heap_capacity = 1;
    }
    struct heap_entry* p = _get_heap_node(addr);
    if (p) {
        p->value = val;
    } else {
        if (heap_size >= heap_capacity) {
            // reallocate
            heap = realloc(heap, (heap_size * 2) * sizeof(struct heap_entry));
            heap_capacity = heap_size * 2;
        }
        heap[heap_size++] = (struct heap_entry){
            addr, val
        };
    }
}

static struct heap_entry* heap_load(ws_int addr) {
    return _get_heap_node(addr);
}

/**
 * @returns NULL on success, valid pointer on error (allocated using malloc)
 * @note For a struct wstree_err* err, free(err) when done.
 * @note err->message is statically allocated. DO NOT FREE IT.
 */
struct wstree_err* wsexecute(struct WS_statement* arr, size_t size) {
    static struct WS_statement **callstack[CALLSTACK_SIZE];
    struct WS_statement **callstack_top = callstack;
    *callstack_top = arr; // first statement onto top of stack
    
    ws_int* stack = malloc(STACK_SIZE * sizeof(ws_int)),
        *stack_top = stack;
    
    struct WS_statement** labels = NULL;
    size_t label_count = 0;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i].op == WS_LABEL) {
            ++label_count;
        }
    }
    if (label_count) {
        labels = malloc(sizeof(struct WS_statement*) * (label_count + 1));
        labels[label_count] = NULL; // sentinel, should we need it
        struct WS_statement** label_p = labels;
        for (size_t i = 0; i < size; ++i) {
            if (arr[i].op == WS_LABEL) {
                *(label_p++) = &arr[i];
            }
        }
    }

    while ((*callstack_top) < arr + size && (*callstack_top)->op != WS_END) {
        struct WS_statement i = **callstack_top;
        // TODO: check if this is still used
        struct WS_statement* p = *callstack_top;
        switch (i.op) {
            case WS_PUSH: {
                if (stack + STACK_SIZE > stack_top) {
                    ++stack_top;
                    *(++stack_top) = i.num;
                } else {
                    e = GET_ERROR_STRUCT(p - arr, "Stack overflow");
                    goto end_program;
                }
                break;
            }
            case WS_DUP: {
                if (stack + STACK_SIZE > stack_top) {
                    *(stack_top + 1) = *stack_top;
                    ++stack_top;
                } else {
                    e = GET_ERROR_STRUCT(p - arr, "Stack overflow");
                    goto end_program;
                }
                break;
            }
            case WS_SWAP: {
                if (stack_top - stack >= 2) {
                    ws_int temp = *stack_top;
                    *stack_top = *(stack_top - 1);
                    *(stack_top - 1) = temp;
                } else {
                    e = GET_ERROR_STRUCT(p - arr, "Stack overflow");
                    goto end_program;
                }
                break;
            }
            case WS_POP: {
                if (stack_top > stack) {
                    --stack_top;
                } else {
                    e = GET_ERROR_STRUCT(p - arr, "Stack underflow");
                    goto end_program;
                }
                break;
            }
            case WS_COPY: {
                if ((stack_top - stack) < i.num) {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for copy");
                    goto end_program;
                } else if (stack_top >= stack + STACK_SIZE) {
                    e = GET_ERROR_STRUCT(p - arr, "Stack overflow");
                    goto end_program;
                } else {
                    ++stack_top;
                    *stack_top = *(stack_top - i.num);
                }
                break;
            }
            case WS_SLIDE: {
                if (stack_top - stack >= (i.num + 1)) {
                    *(stack_top - i.num) = *stack_top;
                    stack_top -= i.num;
                } else {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for slide");
                    goto end_program;
                }
                break;
            }
            case WS_ADD: {
                if (stack_top - stack <= 2) {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for arithmetic operation");
                    goto end_program;
                }
                *(stack_top - 1) += *stack_top;
                --stack_top;
                break;
            }
            case WS_SUB: {
                if (stack_top - stack <= 2) {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for arithmetic operation");
                    goto end_program;
                }
                *(stack_top - 1) -= *stack_top;
                --stack_top;
                break;
            }
            case WS_MUL: {
                if (stack_top - stack <= 2) {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for arithmetic operation");
                    goto end_program;
                }
                *(stack_top - 1) *= *stack_top;
                --stack_top;
                break;
            }
            case WS_DIV: {
                if (stack_top - stack <= 2) {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for arithmetic operation");
                    goto end_program;
                } else if (*stack_top == 0) {
                    e = GET_ERROR_STRUCT(p - arr, "Cannot divide by zero");
                    goto end_program;
                }
                *(stack_top - 1) /= *stack_top;
                --stack_top;
                break;
            }
            case WS_MOD: {
                if (stack_top - stack <= 2) {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for arithmetic operation");
                    goto end_program;
                } else if (*stack_top == 0) {
                    e = GET_ERROR_STRUCT(p - arr, "Cannot divide by zero");
                    goto end_program;
                }
                *(stack_top - 1) %= *stack_top;
                --stack_top;
                break;
            }
            case WS_STORE: {
                if (stack_top - stack <= 2) {
                    e = GET_ERROR_STRUCT(p - arr, "Not enough items on stack for heap store");
                    goto end_program;
                }
                ws_int addr = *(stack_top - 1), val = *stack_top;
                heap_store(addr, val);
                break;
            }
            case WS_LOAD: {
                if (stack_top >= stack + STACK_SIZE) {
                    e = GET_ERROR_STRUCT(p - arr, "Stack overflow");
                    goto end_program;
                }
                ws_int addr = *stack_top;
                struct heap_entry* node = heap_load(addr);
                if (node) {
                    *(++stack_top) = node->value;
                } else {
                    e = GET_FORMATTED_ERROR_STRUCT(p - arr, "Address %jd not found", addr);
                    goto end_program;
                }
                break;
            }
            case WS_LABEL: {
                // no-op, we will handle this in a pass before running the program
                break;
            }
            case WS_CALL: {
                if (!labels) {
                    e = GET_FORMATTED_ERROR_STRUCT(p - arr, "Cannot find label %s", i.label);
                    goto end_program;
                }
                struct WS_statement* label = NULL;
                for (size_t j = 0; j < label_count; ++j) {
                    if (labels[j]->label == i.label) {
                        label = labels[j];
                        break;
                    }
                }
                if (!label) {
                    e = GET_FORMATTED_ERROR_STRUCT(p - arr, "Cannot find label %s", i.label);
                    goto end_program;
                }
                if (callstack_top >= callstack + CALLSTACK_SIZE) {
                    e = GET_ERROR_STRUCT(p - arr, "Callstack overflow");
                    goto end_program;
                }
                *(++callstack_top) = label;
                break;
            }
            case WS_JMP: {
                goto jump;
                break;
            }
            case WS_JZ: {
                if (*stack_top == 0) 
                    goto jump;
                break;
            }
            case WS_JLZ: {
                if (*stack_top < 0) 
                    goto jump;
                break;
            }
            case WS_RET: {
                if (callstack_top <= callstack) {
                    e = GET_ERROR_STRUCT(p - arr, "Nothing to return");
                    goto end_program;
                }
                --callstack_top;
                break;
            }
            case WS_END: {
                goto end_program;
            }
            case WS_OUTCHR: {
                if (stack >= stack_top) {
                    e = GET_ERROR_STRUCT(p - arr, "stack underflow");
                    goto end_program;
                }
                putchar(*stack_top);
                break;
            }
            case WS_OUTNUM: {
                if (stack >= stack_top) {
                    e = GET_ERROR_STRUCT(p - arr, "stack underflow");
                    goto end_program;
                }
                printf("%jd\n", *stack_top);
                break;
            }
            case WS_INCHR: {
                if (stack >= stack_top) {
                    e = GET_ERROR_STRUCT(p - arr, "stack underflow");
                    goto end_program;
                }
                int c;
                if ((c = getchar()) != EOF) {
                    heap_store(*stack_top, c);
                } else {
                    e = GET_ERROR_STRUCT(p - arr, "IO error. Check errno and/or EOF/error flags of stdin");
                    goto end_program;
                }
                break;
            }
            case WS_INNUM: {
                if (stack >= stack_top) {
                    e = GET_ERROR_STRUCT(p - arr, "stack underflow");
                    goto end_program;
                }
                ws_int n;
                if (scanf("%jd\n", &n)) {
                    heap_store(*stack_top, n);
                } else {
                    e = GET_ERROR_STRUCT(p - arr, "Invalid number / IO error. Check errno and/or EOF/error flags of stdin");
                    goto end_program;
                }
                break;
            }
            default: {
                e = GET_FORMATTED_ERROR_STRUCT(p - arr, "Unknown option %d", i.op);
                goto end_program;
            }
            jump:
                if (!labels) {
                    e = GET_FORMATTED_ERROR_STRUCT(p - arr, "Cannot find label %s", i.label);
                    goto end_program;
                }
                struct WS_statement* label = NULL;
                for (size_t j = 0; j < label_count; ++j) {
                    if (labels[j]->label == i.label) {
                        label = labels[j];
                        break;
                    }
                }
                if (!label) {
                    e = GET_FORMATTED_ERROR_STRUCT(p - arr, "Cannot find label %s", i.label);
                    goto end_program;
                }
                *callstack_top = label;

        }
        ++(*callstack_top);
    }
    if ((*callstack_top) >= arr + size) {
        fprintf(stderr, "Warning: reached end of program but didn't find END instruction\n");
    }
    end_program:
    free(stack);
    if (labels) free(labels);
    return e;
}

#undef GET_ERROR_STRUCT
#undef GET_FORMATTED_ERROR_STRUCT
