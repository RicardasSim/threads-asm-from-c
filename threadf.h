#ifndef THREADF_H
#define THREADF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t TestFnAsm1 ( int8_t*, uint64_t);
uint64_t TestFnAsm2 ( int8_t*, uint64_t);
uint64_t TestFnAsm3 ( int8_t*, uint64_t, int8_t*);
uint64_t TestFnAsm4 ( int8_t*, uint64_t);

#ifdef __cplusplus
}
#endif

#endif // THREADF_H
