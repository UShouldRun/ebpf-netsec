#ifndef __PAIR_H__
#define __PAIR_H__

#include <stdint.h>
#include "utils.h"

#define GET_MACRO_2(_1, _2, NAME, ...) NAME
#define GET_MACRO_4(_1, _2, _3, _4, NAME, ...) NAME

#define pair(...) \
  GET_MACRO_2(__VA_ARGS__, pair2, pair1)(__VA_ARGS__)

#define pair_declare(...) \
  GET_MACRO_2(__VA_ARGS__, pair_declare2, pair_declare1)(__VA_ARGS__)

#define pair_init(...) \
  GET_MACRO_4(__VA_ARGS__, pair_init2, pair_init1)(__VA_ARGS__)

#define PAIR_NULL { 0 }

#define pair1(type)         pair_##type##_##type
#define pair2(type1, type2) pair_##type1##_##type2

#define pair_declare1(type) \
  typedef struct { \
    type first, second; \
  } pair(type);

#define pair_declare2(type1, type2) \
  typedef struct { \
    type1 first; \
    type2 second; \
  } pair(type1, type2);

#define pair_init1(type, first_val, second_val) \
  (pair(type)){ .first = first_val, .second = second_val }
#define pair_init2(type1, type2, first_val, second_val) \
  (pair(type1, type2)){ .first = first_val, .second = second_val }

pair_declare(i8)
pair_declare(i16)
pair_declare(i32)
pair_declare(i64)

pair_declare(u8)
pair_declare(u16)
pair_declare(u32)
pair_declare(u64)

pair_declare(f32)
pair_declare(f64)

#endif // !__PAIR_H__
