#include <stdio.h>
#include <sys/types.h>

#ifndef ERR_BUF_SIZE
#define ERR_BUF_SIZE 127
#endif

enum Err_Type {
    ERR_NONE = 0,
    ERR_PARSE = 1,
    ERR_RUNTIME = 2
};

extern const char* Err_Type_to_string[];

struct Err_Error {
    enum Err_Type type;
    size_t index; // not all errors use this
    const char* message;
};

typedef struct Err_Error Err_Error;

Err_Error* Err_setError(enum Err_Type type, size_t i, const char* m);
Err_Error* Err_setErrorFromFormat(enum Err_Type type, size_t i, const char* format, ...);
Err_Error* Err_getError(void);
int Err_isSet(void);
void Err_clearError(void);
int Err_perror(void);
int Err_fperror(FILE* stream);
