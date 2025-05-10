#include <sys/types.h>

enum Err_Type {
    ERR_NONE = 0,
    ERR_PARSE = 1,
    ERR_RUNTIME = 2
};

struct Err_Error {
    enum Err_Type type;
    size_t index; // not all errors use this
    const char* message;
};

typedef struct Err_Error Err_Error;

Err_Error* Err_setError(enum Err_Type type, size_t i, const char* m);
Err_Error* Err_setErrorFromFormat(enum Err_Type type, size_t i, const char* format, ...);
Err_Error* Err_getError();
int Err_isSet();
void Err_clearError();
