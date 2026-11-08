#ifndef __STRING8_H__
#define __STRING8_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

#include <alloc/alloc.h>
#include <vector/vector.h>
#include "utils.h"

typedef struct string8 str8;

struct string8 {
  u64 s_str;
  u8* str;
};

vector_declare(str8)

#define STR8_LIT(s)  ((str8){ .s_str = sizeof(s) - 1, .str = (u8*)s })
#define STR8_FMT(s)  (i32)(s).s_str, (char*)(s).str
#define STR8_NULL    ((str8){ .s_str = 0, .str = NULL })

static inline str8 string8_create(const u64 s_str, u8* str) {
  return (str8){
    .s_str = s_str,
    .str   = str 
  };
}

i8            string8_compare        (const str8 a, const str8 b);

bool          string8_starts_with    (const str8 source, const str8 expr);
bool          string8_ends_with      (const str8 source, const str8 expr);
bool          string8_contains       (const str8 source, const str8 expr);
bool          string8_is_alpha       (const str8 source);
bool          string8_is_num         (const str8 source);
bool          string8_is_alpha_num   (const str8 source);

void          string8_to_lower       (str8 target, const str8 source);
void          string8_to_upper       (str8 target, const str8 source);
void          string8_to_title       (str8 target, const str8 source);
void          string8_reverse        (str8 target, const str8 source);

vector(u64)   string8_indexes_of     (const str8 source, const str8 expr);
u64           string8_count_occur    (const str8 source, const str8 expr);
u64           string8_count_words    (const str8 source);

// ============================# String8 Malloc #===============================

str8          string8_copy            (const str8 source);
str8          string8_trim            (const str8 source);
str8          string8_strip           (const str8 source);
str8          string8_remove          (const str8 source, const str8 expr);
vector(str8)  string8_split           (const str8 source, const str8 expr);
str8          string8_join            (const vector(str8) source);
str8          string8_replace         (const str8 source, const str8 find_expr, const str8 replace_expr);
str8          string8_repeat          (const str8 source, const u64 n);
str8          string8_truncate        (const str8 source, const u64 pos);
str8          string8_pad_start       (const str8 source, const str8 pad);
str8          string8_pad_end         (const str8 source, const str8 pad);
str8          string8_substring       (const str8 source, const u64 start, const u64 end);
str8          string8_format          (const char* fmt, ...);
str8          string8_vformat         (const char* fmt, va_list args);
str8          string8_read_file       (const char* filename);

// ============================# String8 Arena Alloc #===============================

str8          string8_copy_arena      (const str8 source, Arena* arena);
str8          string8_trim_arena      (const str8 source, Arena* arena);
str8          string8_strip_arena     (const str8 source, Arena* arena);
str8          string8_remove_arena    (const str8 source, const str8 expr, Arena* arena);
vector(str8)  string8_split_arena     (const str8 source, const str8 expr, Arena* arena);
str8          string8_join_arena      (const vector(str8) source, Arena* arena);
str8          string8_replace_arena   (const str8 source, const str8 find_expr, const str8 replace_expr, Arena* arena);
str8          string8_repeat_arena    (const str8 source, const u64 n, Arena* arena);
str8          string8_truncate_arena  (const str8 source, const u64 pos, Arena* arena);
str8          string8_pad_start_arena (const str8 source, const str8 pad, Arena* arena);
str8          string8_pad_end_arena   (const str8 source, const str8 pad, Arena* arena);
str8          string8_substring_arena (const str8 source, const u64 start, const u64 end, Arena* arena);
str8          string8_format_arena    (Arena* arena, const char* fmt, ...);
str8          string8_vformat_arena   (Arena* arena, const char* fmt, va_list args);
str8          string8_read_file_arena (const char* filename, Arena* arena);
//
// ============================# String8 Pool Alloc #===============================

str8          string8_copy_pool       (const str8 source, Pool* pool);
str8          string8_trim_pool       (const str8 source, Pool* pool);
str8          string8_strip_pool      (const str8 source, Pool* pool);
str8          string8_remove_pool     (const str8 source, const str8 expr, Pool* pool);
vector(str8)  string8_split_pool      (const str8 source, const str8 expr, Pool* pool);
str8          string8_join_pool       (const vector(str8) source, Pool* pool);
str8          string8_replace_pool    (const str8 source, const str8 find_expr, const str8 replace_expr, Pool* pool);
str8          string8_repeat_pool     (const str8 source, const u64 n, Pool* pool);
str8          string8_truncate_pool   (const str8 source, const u64 pos, Pool* pool);
str8          string8_pad_start_pool  (const str8 source, const str8 pad, Pool* pool);
str8          string8_pad_end_pool    (const str8 source, const str8 pad, Pool* pool);
str8          string8_substring_pool  (const str8 source, const u64 start, const u64 end, Pool* pool);
str8          string8_format_pool     (Pool* pool, const char* fmt, ...);
str8          string8_vformat_pool    (Pool* pool, const char* fmt, va_list args);
str8          string8_read_file_pool  (const char* filename, Pool* pool);

#endif // !__STRING8_H__
