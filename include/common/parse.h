#ifndef PARSE_H
#define PARSE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/types.h>

struct WS_statement* WS_parse(const char*, size_t*);


#ifdef __cplusplus
}
#endif

#endif // PARSE_H
