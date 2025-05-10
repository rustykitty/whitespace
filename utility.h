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

#ifndef MAX
#define MAX(__x, __y) ({               \
    __typeof__(__x) __a = __x;         \
    __typeof__(__y) __b = __y;         \
    __a > __b ? __a : __b;             \
})
#endif

#ifndef MIN
#define MIN(__x, __y) ({               \
    __typeof__(__x) __a = __x;         \
    __typeof__(__y) __b = __y;         \
    __a < __b ? __a : __b;             \
})
#endif

#endif
