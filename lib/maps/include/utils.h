#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * @file utils.h
 * @brief Integer aliases for both compilation worlds
 *
 * - Kernel side: vmlinux.h already defines u8..u64 (and bool), so only the signed aliases are added.
 * - User side:   the usual stdint aliases.
 */

#ifdef __bpf__

typedef s8  i8;
typedef s16 i16;
typedef s32 i32;
typedef s64 i64;

#else

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;

#endif /* __bpf__ */

#endif // !__UTILS_H__
