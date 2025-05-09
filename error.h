#include <sys/types.h>

struct wstree_err {
    size_t index;
    const char* message;
};

typedef struct wstree_err wstree_err;

wstree_err* get_error_struct(size_t i, const char* m);
wstree_err* get_formatted_error_struct(size_t i, const char* format, ...);
