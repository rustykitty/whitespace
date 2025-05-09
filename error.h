#include <sys/types.h>

struct Err_Error {
    size_t index;
    const char* message;
};

typedef struct Err_Error Err_Error;

Err_Error* Err_setError(size_t i, const char* m);
Err_Error* Err_setErrorFromFormat(size_t i, const char* format, ...);
Err_Error* Err_getError();
int Err_isSet();
void Err_clearError();