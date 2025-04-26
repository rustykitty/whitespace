#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

#ifndef NDEBUG
#define DEBUG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG(...)
#endif

#ifndef ALWAYS_INLINE
#if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#define ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE
#endif
#endif

struct wstree_err {
    size_t index;
    const char* message;
};

char err_buf[127];

#define GET_ERROR_STRUCT(_i, _m) ({      \
    strcpy(err_buf, _m);                     \
    struct wstree_err* _err = malloc(sizeof(struct wstree_err)); \
    *_err = (struct wstree_err){             \
        _i, err_buf                      \
    };                                       \
    _err;                                    \
})

#define GET_FORMATTED_ERROR_STRUCT( _i, ...) ({ \
    char _tmp[sizeof(err_buf)];               \
    snprintf(_tmp, sizeof(err_buf), __VA_ARGS__); \
    GET_ERROR_STRUCT(_i, _tmp);                \
})

#ifndef MAX
#define MAX(x, y) (x > y ? x : y)
#endif

#ifndef MIN
#define MIN(x, y) (x < y ? x : y)
#endif

#endif
