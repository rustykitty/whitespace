#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>

#include "whitespace.h"
#include "utility.h"
#include "exec_tree.h"
#include "error.h"

#ifndef INITIAL_STACK_SIZE
#define INITIAL_STACK_SIZE 512
#endif

#ifndef CALLSTACK_SIZE
#define CALLSTACK_SIZE 128
#endif

#ifndef INITIAL_HEAP_SIZE
#define INITIAL_HEAP_SIZE 256
#endif

struct heap_entry {
    size_t address;
    ws_int value;
};

ws_int* heap = NULL;
size_t heap_size;

static inline ALWAYS_INLINE ws_int* heap_init() {
    heap = calloc(INITIAL_HEAP_SIZE, sizeof(size_t));
    heap_size = INITIAL_HEAP_SIZE;
    return heap;
}

static inline ALWAYS_INLINE void heap_free() {
    free(heap);
    heap = NULL;
    heap_size = 0;
}

#define MOST_SIGNIFICANT_BIT_SET(__n)  \
     ({                                \
        unsigned char __res = 0;       \
        while (__n) {                  \
            __n <<= 1;                 \
            __res++;                   \
            }                          \
        __res;                         \
     })

#define POWER_OF_TWO(__n) (__n == 0 ? 1 : 1ULL << (__n - 1))

static inline void check_heap_space(size_t address) {
    if (address >= heap_size) {
        size_t old_size = heap_size;
        size_t new_size = POWER_OF_TWO(MOST_SIGNIFICANT_BIT_SET(address) + 1);
        heap = realloc(heap, new_size * sizeof(ws_int));
        memset(heap + old_size, 0, (new_size - old_size) * sizeof(ws_int));
    }
}

/**
 * Store a value into the heap.
 * @param addr The address in the heap to store the value at
 * @param val The value to store in the address
 */
static void heap_store(ws_int addr, ws_int val) {
    size_t address = (size_t)addr;
    check_heap_space(address);
    heap[address] = val;
}

/**
 * Loads a value from the heap.
 * @param addr The address to load from.
 * @returns A pointer to a valid heap entry if the address exists in the heap, NULL otherwise.
 */
static inline ws_int heap_load(ws_int addr) {
    check_heap_space((size_t)addr);
    return heap[(size_t)addr];
}

static inline ALWAYS_INLINE int labelcmp(label_type a, label_type b) {
    return strcmp(a, b) == 0;
}

static inline struct WS_statement* get_label(struct WS_statement** labels, size_t label_count, label_type label) {
    if (!labels) {
        return NULL;
    }
    for (size_t i = 0; i < label_count; ++i) {
        if (labelcmp(labels[i]->label, label)) {
            return labels[i];
            break;
        }
    }
    return NULL; // not found
}

/**
 * Called from whitespace_module to execute the parsed instructions after converting from Python
 * @returns NULL on success, valid pointer on error (allocated using malloc)
 * @note For a struct wstree_err* err, free(err) when done.
 * @note err->message is statically allocated. DO NOT FREE IT.
 */
struct wstree_err* wsexecute(struct WS_statement* arr, size_t size) {
    static struct wstree_err* e = NULL;
    static struct WS_statement *callstack[CALLSTACK_SIZE];
    struct WS_statement **callstack_top = callstack;
    *callstack_top = arr; // first statement onto top of stack
    
    ws_int* stack = malloc(INITIAL_STACK_SIZE * sizeof(ws_int)),
        *stack_top = stack;
    size_t stack_size = INITIAL_STACK_SIZE;

    heap_init();

    struct WS_statement** labels = NULL;
    size_t label_count = 0;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i].op == WS_LABEL) {
            ++label_count;
        }
    }
    if (label_count > 0) {
        labels = malloc(sizeof(struct WS_statement*) * (label_count + 1));
        labels[label_count] = NULL; // sentinel, should we need it
        size_t x = 0;
        for (size_t i = 0; i < size; ++i) {
            if (arr[i].op == WS_LABEL) {
                labels[x] = &arr[i];
                ++x;
            }
        }
    }

    for (size_t i = 0; i < size; ++i) {
        if (arr[i].op == WS_CALL ||
            arr[i].op == WS_JMP ||
            arr[i].op == WS_JZ ||
            arr[i].op == WS_JLZ) {
            struct WS_statement* label = get_label(labels, label_count, arr[i].label);
            if (!label) {
                e = get_formatted_error_struct(i, "Label %s was used but never defined", arr[i].label);
                goto end_program;
            }
            arr[i].label_ptr = label;
        }
    }

    while ((*callstack_top) < arr + size && (*callstack_top)->op != WS_END) {
        struct WS_statement i = **callstack_top;
        struct WS_statement* p = *callstack_top;
        switch (i.op) {
        case WS_PUSH: {
            if ((size_t)(stack_top - stack) >= stack_size) {
                stack_size = stack_size * 2;
                stack = realloc(stack, stack_size);
            }
            ++stack_top;
            *stack_top = i.num;
            break;
        }
        case WS_DUP: {
            if ((size_t)(stack_top - stack) >= stack_size) {
                stack_size = stack_size * 2;
                stack = realloc(stack, stack_size);
            }
            stack_size = stack_size * 2;
            stack = realloc(stack, stack_size);
            break;
        }
        case WS_SWAP: {
            if (stack_top - stack < 2) {
                e = get_error_struct(p - arr, "Not enough items on stack for swap");
                goto end_program;
            }
            ws_int temp = *stack_top;
            *stack_top = *(stack_top - 1);
            *(stack_top - 1) = temp;
            break;
        }
        case WS_POP: {
            if (stack_top <= stack) {
                e = get_error_struct(p - arr, "Not enough items on stack for pop");
                goto end_program;
            }
            --stack_top;
            break;
        }
        case WS_COPY: {
            if (stack_top - stack < i.num) {
                e = get_error_struct(p - arr, "Not enough items on stack for copy");
                goto end_program;
            } else {
                if ((size_t)(stack_top - stack) >= stack_size) {
                    stack_size = stack_size * 2;
                    stack = realloc(stack, stack_size);
                }
                ++stack_top;
                *stack_top = *(stack_top - i.num);
            }
            break;
        }
        case WS_SLIDE: {
            if (stack_top >= stack) {
                ws_int top_value = *stack_top; 
                ws_int* new_top = MAX(stack_top - i.num, stack);
                *(stack_top = new_top) = top_value;
            } else {
                e = get_error_struct(p - arr, "Not enough items on stack for slide");
                goto end_program;
            }
            break;
        }
        case WS_ADD: {
            if (stack_top - stack <= 2) {
                e = get_error_struct(p - arr, "Not enough items on stack for arithmetic operation");
                goto end_program;
            }
            *(stack_top - 1) += *stack_top;
            --stack_top;
            break;
        }
        case WS_SUB: {
            if (stack_top - stack <= 2) {
                e = get_error_struct(p - arr, "Not enough items on stack for arithmetic operation");
                goto end_program;
            }
            *(stack_top - 1) -= *stack_top;
            --stack_top;
            break;
        }
        case WS_MUL: {
            if (stack_top - stack <= 2) {
                e = get_error_struct(p - arr, "Not enough items on stack for arithmetic operation");
                goto end_program;
            }
            *(stack_top - 1) *= *stack_top;
            --stack_top;
            break;
        }
        case WS_DIV: {
            if (stack_top - stack <= 2) {
                e = get_error_struct(p - arr, "Not enough items on stack for arithmetic operation");
                goto end_program;
            } else if (*stack_top == 0) {
                e = get_error_struct(p - arr, "Cannot divide by zero");
                goto end_program;
            }
            *(stack_top - 1) /= *stack_top;
            --stack_top;
            break;
        }
        case WS_MOD: {
            if (stack_top - stack <= 2) {
                e = get_error_struct(p - arr, "Not enough items on stack for arithmetic operation");
                goto end_program;
            } else if (*stack_top == 0) {
                e = get_error_struct(p - arr, "Cannot divide by zero");
                goto end_program;
            }
            *(stack_top - 1) %= *stack_top;
            --stack_top;
            break;
        }
        case WS_STORE: {
            if (stack_top - stack <= 2) {
                e = get_error_struct(p - arr, "Not enough items on stack for heap store");
                goto end_program;
            }
            ws_int addr = *(stack_top - 1), val = *stack_top;
            if ((size_t)addr > SIZE_MAX) {
                e = get_error_struct(p - arr, "Heap address too large");
                goto end_program;
            }
            heap_store(addr, val);
            break;
        }
        case WS_LOAD: {
            ws_int addr = *stack_top;
            if ((size_t)addr > SIZE_MAX) {
                e = get_error_struct(p - arr, "Heap address too large");
                goto end_program;
            }
            ws_int val = heap_load(addr);
            if ((size_t)(stack_top - stack) >= stack_size) {
                stack_size = stack_size * 2;
                stack = realloc(stack, stack_size);
            }
            ++stack_top;
            *stack_top = val;
            break;
        }
        case WS_LABEL: {
            // no-op, labels are precomputed immediately before runtime
            break;
        }
        case WS_CALL: {
            if (callstack_top >= callstack + CALLSTACK_SIZE) {
                e = get_error_struct(p - arr, "Callstack overflow");
                goto end_program;
            }
            *(++callstack_top) = i.label_ptr;
            break;
        }
        case WS_JMP: {
            *callstack_top = i.label_ptr;
            break;
        }
        case WS_JZ: {
            if (*stack_top == 0)
                *callstack_top = i.label_ptr;
            break;
        }
        case WS_JLZ: {
            if (*stack_top < 0)
                *callstack_top = i.label_ptr;
            break;
        }
        case WS_RET: {
            if (callstack_top <= callstack) {
                e = get_error_struct(p - arr, "Nothing to return");
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
                e = get_error_struct(p - arr, "stack underflow");
                goto end_program;
            }
            putchar(*stack_top);
            break;
        }
        case WS_OUTNUM: {
            if (stack >= stack_top) {
                e = get_error_struct(p - arr, "stack underflow");
                goto end_program;
            }
            printf("%jd\n", *stack_top);
            break;
        }
        case WS_INCHR: {
            if (stack >= stack_top) {
                e = get_error_struct(p - arr, "stack underflow");
                goto end_program;
            }
            int c;
            if ((c = getchar()) != EOF || feof(stdin)) { // EOF is okay
                heap_store(*stack_top, c);
            } else if (ferror(stdin)) {
                e = get_formatted_error_struct(p - arr, "Errno %d: %s", errno, strerror(errno));
                goto end_program;
            }
            break;
        }
        case WS_INNUM: {
            if (stack >= stack_top) {
                e = get_error_struct(p - arr, "stack underflow");
                goto end_program;
            }
            ws_int n;
            if (scanf("%jd\n", &n)) {
                heap_store(*stack_top, n);
            } else {
                /* It's kinda hard to tell which one... */
                e = get_formatted_error_struct(p - arr, "Invalid number / IO error: %s", strerror(errno));
                goto end_program;
            }
            break;
        }
        default: {
            e = get_formatted_error_struct(p - arr, "Unknown option %d", i.op);
            goto end_program;
        }
        }
        ++(*callstack_top);
    }
    if ((*callstack_top) >= arr + size) {
        fprintf(stderr, "Warning: reached end of program but didn't find END instruction\n");
    }
    end_program:
    free(stack);
    if (labels) free(labels);
    heap_free();
    return e;
}
