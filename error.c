#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

#ifndef ERR_BUF_SIZE
#define ERR_BUF_SIZE 127
#endif

static char err_buf[ERR_BUF_SIZE];
static wstree_err err_s;

wstree_err* get_error_struct(size_t i, const char* m) {
    strcpy(err_buf, m);
    err_s = (wstree_err){
        .index = i, 
        .message = err_buf
    };
    return &err_s;
}

wstree_err* get_formatted_error_struct(size_t i, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(err_buf, sizeof(err_buf), format, args);
    va_end(args);
    
    err_s = (wstree_err){
        .index = i,
        .message = err_buf
    };
    return &err_s;
}