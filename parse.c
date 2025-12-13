#include "ws_error.h"
#include "whitespace.h"
#include "utility.h"
#include "parse.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const char* whitespace_instructions[] = {
    [WS_PUSH] = "  ",
    [WS_DUP] = " \n ",
    [WS_COPY] = " \t ",
    [WS_SWAP] = " \n\t",
    [WS_POP] = " \n\n",
    [WS_SLIDE] = " \t\n",
    [WS_ADD] = "\t   ",
    [WS_SUB] = "\t  \t",
    [WS_MUL] = "\t  \n",
    [WS_DIV] = "\t \t ",
    [WS_MOD] = "\t \t\t",
    [WS_STORE] = "\t\t ",
    [WS_LOAD] = "\t\t\t",
    [WS_LABEL] = "\n  ",
    [WS_CALL] = "\n \t",
    [WS_JMP] = "\n \n",
    [WS_JZ] = "\n\t ",
    [WS_JLZ] = "\n\t\t",
    [WS_RET] = "\n\t\n",
    [WS_END] = "\n\n\n",
    [WS_OUTCHR] = "\t\n  ",
    [WS_OUTNUM] = "\t\n \t",
    [WS_INCHR] = "\t\n\t\n",
    [WS_INNUM] = "\t\n\t\t",
};

static ssize_t find_instruction(const char** str_p) {
    const char* restrict str = *str_p;
    for (size_t i = 0; i < sizeof(whitespace_instructions) / sizeof(whitespace_instructions[0]); ++i) {
        if (!whitespace_instructions[i]) {
            continue;
        }
        if (strncmp(str, whitespace_instructions[i], strlen(whitespace_instructions[i])) == 0) {
            *str_p += strlen(whitespace_instructions[i]);
            return i;
        }
    }
    Err_setErrorFromFormat(ERR_PARSE, 0, "Unexpected end of input", str);
    return -1;
}

static ws_int parse_number(const char** str_p) {
    const char* restrict str = *str_p;
    ws_int num = 0;
    if (*str == '\n') {
        Err_setError(ERR_PARSE, 0, "Unexpected end of number literal");
        return 0;
    }
    bool negative = *str == '\t';
    ++str;
    while (*str && *str != '\n') {
        num = (num << 1) | (*str == '\t');
        ++str;
    }
    *str_p = str + 1;
    return negative ? -num : num;
}

static label_type parse_label(const char** str_p) {
    const char* restrict str = *str_p;
    const char* memchr_result = (char*)memchr(str, '\n', strlen(str));
    if (!memchr_result) {
        Err_setError(ERR_PARSE, 0, "Unexpected end of input");
        return NULL;
    }
    *str_p = memchr_result + 1;
    size_t length = memchr_result - str;
    char* restrict label = calloc(length + 1, 1);
    for (size_t i = 0; str[i] && str[i] != '\n'; ++i) {
        label[i] = (str[i] == '\t') ? '1' : '0';
    }
    return label;
}

static char* minify(const char* restrict str) {
    if (!str) {
        return NULL;
    }
    size_t len = strlen(str);
    char* result = calloc(len + 1, 1);
    {
        char* restrict p = result;
        while (*str) {
            if (*str == ' ' || *str == '\t' || *str == '\n') {
                *(p++) = *str;
            }
            ++str;
        }
    }
    result = realloc(result, strlen(result) + 1);
    return result;
}

struct WS_statement* WS_parse(const char* str, size_t* size_p) {
    // overalloc on purpose, so we don't need to realloc many times (no dynamic array!)
    const char* minified = minify(str);
    // shortest instruction is 3 bytes long
    struct WS_statement* restrict arr = calloc((strlen(str) / 3) + 2, sizeof(struct WS_statement));

    const char* const beg = minified;

    const char* p = minified;
    size_t i = 0;
    while (*p) {
        DEBUG("[DEBUG] %zd\n", p - beg);
        ssize_t instruction = find_instruction(&p);
        if (instruction < 0) {
            Err_getError()->index = p - beg;
            goto error;
        }
        struct WS_statement statement = {
            .op = instruction
        };
        DEBUG("[DEBUG] Got instruction %zd\n", instruction);
        switch (instruction) {
        case WS_PUSH:
        case WS_COPY:
        case WS_SLIDE:
            {
                const ws_int n = parse_number(&p);
                DEBUG("[DEBUG] Parsed number %zd\n", n);
                if (Err_isSet()) {
                    Err_getError()->index = p - beg;
                    goto error;
                }
                statement.num = n;
                break;
            }
        case WS_LABEL:
        case WS_CALL:
        case WS_JMP:
        case WS_JZ:
        case WS_JLZ:
            {
                label_type label = parse_label(&p);
                if (!label) {
                    Err_getError()->index = p - beg;
                    goto error;
                }
                DEBUG("[DEBUG] Parsed label %s\n", label);
                statement.label = label;
                break;
            }
        default: 
        break;
        }
        arr[i] = statement;
        ++i;
    }

    arr = realloc(arr, i * sizeof(struct WS_statement));

    *size_p = i;

    free((void*)minified);

    return arr;

    error:
    free((void*)minified);
    free(arr);
    return NULL;
}