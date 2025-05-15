#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

#ifndef ERR_BUF_SIZE
#define ERR_BUF_SIZE 127
#endif

extern const char* Err_Type_to_string[] = {
    [ERR_NONE] = "ERR_NONE",
    [ERR_PARSE] = "ERR_PARSE",
    [ERR_RUNTIME] = "ERR_RUNTIME"
};

static char err_buf[ERR_BUF_SIZE];
static Err_Error err_s = {
    .index = 0,
    .message = NULL
};

Err_Error* Err_setError(enum Err_Type type, size_t i, const char* m) {
    strcpy(err_buf, m);
    err_s = (Err_Error){
        .type = type,
        .index = i, 
        .message = err_buf
    };
    return &err_s;
}

Err_Error* Err_setErrorFromFormat(enum Err_Type type, size_t i, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(err_buf, sizeof(err_buf), format, args);
    va_end(args);
    
    err_s = (Err_Error){
        .type = type,
        .index = i,
        .message = err_buf
    };
    return &err_s;
}

Err_Error* Err_getError() {
    return &err_s;
}

int Err_isSet() {
    return err_s.message != NULL;
}

void Err_clearError() {
    err_s = (Err_Error){
        .index = 0,
        .message = NULL
    };
}

int Err_perror()  {
    return Err_perror(stderr);
}

int Err_fperror(FILE* stream) {
    if (Err_isSet()) {
        return fprintf(stream, "%s: %s\n", Err_Type_to_string[err_s.type], err_s.message || "");
    }
    return 0;
}
