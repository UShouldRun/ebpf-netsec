#include "string8_private.h"

// ============================# PUBLIC #===============================

i8 string8_compare(const str8 a, const str8 b) {
  if (a.str == NULL && b.str == NULL)
    return 0;
  if (a.str == NULL)
    return 1;
  if (b.str == NULL)
    return -1;

  const u64 min_size = a.s_str <= b.s_str ? a.s_str : b.s_str;

  for (u64 i = 0; i < min_size; i++)
    if (a.str[i] != b.str[i])
      return a.str[i] < b.str[i] ? -1 : 1;

  return (a.s_str > b.s_str) - (a.s_str < b.s_str);
}

bool string8_starts_with(const str8 source, const str8 expr) {
  if (source.str == NULL || expr.str == NULL)
    return false;

  const u64 s_expr = expr.s_str;

  for (u64 i = 0; i < s_expr; i++)
    if (source.str[i] != expr.str[i])
      return false;

  return true;
}

bool string8_ends_with(const str8 source, const str8 expr) {
  if (source.str == NULL || expr.str == NULL)
    return false;
  if (expr.s_str > source.s_str)
    return false;

  const u64 offset = source.s_str - expr.s_str;
  for (u64 i = 0; i < expr.s_str; i++)
    if (source.str[offset + i] != expr.str[i])
      return false;

  return true;
}

bool string8_contains(const str8 source, const str8 expr) {
  if (source.str == NULL || expr.str == NULL)
    return false;

  const u64 
    s_str  = source.s_str,
    s_expr = expr.s_str;

  vector(u64) lps = __string8_build_kmp_lps(expr);
  if (vector_is_empty(u64, &lps))
    return false;

  u64
    str_index  = 0,
    expr_index = 0;

  bool contains = false;
  while (str_index < s_str) {
    if (expr.str[expr_index] == source.str[str_index]) {
      expr_index++;
      str_index++;

      if (expr_index == s_expr) {
        contains = true;
        break;
      }

      continue;
    }

    if (expr_index > 0) {
      expr_index = lps.array[expr_index - 1];
      continue;
    }

    str_index++;
  }

  vector_free(u64, &lps);
  return contains;
}

bool string8_is_alpha(const str8 source) {
  if (source.str == NULL)
    return false;

  const u64 s_str = source.s_str;
  for (u64 i = 0; i < s_str; i++)
    if (!__string8_char_is_alpha(source.str[i]))
      return false;

  return true;
}

bool string8_is_num(const str8 source) {
  if (source.str == NULL)
    return false;

  const u64 s_str = source.s_str;
  for (u64 i = 0; i < s_str; i++)
    if (!__string8_char_is_num(source.str[i]))
      return false;

  return true;
}

bool string8_is_alpha_num(const str8 source) {
  if (source.str == NULL)
    return false;

  const u64 s_str = source.s_str;
  for (u64 i = 0; i < s_str; i++)
    if (!(
      __string8_char_is_alpha(source.str[i]) ||
      __string8_char_is_num(source.str[i])
    ))
      return false;

  return true;
}

void string8_to_lower(str8 target, const str8 source) {
  if (target.str == NULL || source.str == NULL)
    return;

  if (target.s_str != source.s_str)
    return;

  const u64 s_str = target.s_str;
  for (u64 i = 0; i < s_str; i++)
    target.str[i] = __string8_char_to_lower(source.str[i]);
}

void string8_to_upper(str8 target, const str8 source) {
  if (target.str == NULL || source.str == NULL)
    return;

  if (target.s_str != source.s_str)
    return;

  const u64 s_str = target.s_str;
  for (u64 i = 0; i < s_str; i++)
    target.str[i] = __string8_char_to_upper(source.str[i]);
}

void string8_to_title(str8 target, const str8 source) {
  if (target.str == NULL || source.str == NULL)
    return;

  if (target.s_str != source.s_str)
    return;

  const u64 s_str = target.s_str;
  bool whitespace = true;
  for (u64 i = 0; i < s_str; i++) {
    target.str[i] = 
      whitespace ? 
        __string8_char_to_upper(source.str[i]) :
        source.str[i];

    whitespace = __string8_char_is_whitespace(source.str[i]);
  }
}

void string8_reverse(str8 target, const str8 source) {
  if (target.str == NULL || source.str == NULL)
    return;

  if (target.s_str != source.s_str)
    return;

  const u64 s_str = target.s_str;
  for (u64 i = 0; i < s_str; i++)
    target.str[i] = source.str[s_str - 1 - i];
}

vector(u64) string8_indexes_of(const str8 source, const str8 expr) {
  // Implementation of the KMP Search Algorithm
  if (source.str == NULL || expr.str == NULL)
    return VECTOR_NULL(u64);

  vector(u64) matches = vector_init(u64, 0, 0);

  const u64 
    s_str  = source.s_str,
    s_expr = expr.s_str;

  vector(u64) lps = __string8_build_kmp_lps(expr);
  if (vector_is_empty(u64, &lps))
    return VECTOR_NULL(u64);

  u64
    source_index = 0,
    expr_index   = 0;

  while (source_index < s_str) {
    if (expr.str[expr_index] == source.str[source_index]) {
      expr_index++;
      source_index++;

      if (expr_index == s_expr) {
        vector_append(u64, &matches, source_index - s_expr);
        expr_index = lps.array[s_expr - 1];
      }

      continue;
    }

    if (expr_index > 0) {
      expr_index = lps.array[expr_index - 1];
      continue;
    }

    source_index++;
  }

  vector_free(u64, &lps);
  return matches;
}

u64 string8_count_occur(const str8 source, const str8 expr) {
  // Implementation of the KMP Search Algorithm
  if (source.str == NULL || expr.str == NULL)
    return 0;

  u64 count = 0;

  const u64 
    s_str  = source.s_str,
    s_expr = expr.s_str;

  vector(u64) lps = __string8_build_kmp_lps(expr);
  if (vector_is_empty(u64, &lps))
    return 0;

  u64
    str_index  = 0,
    expr_index = 0;

  while (str_index < s_str) {
    if (expr.str[expr_index] == source.str[str_index]) {
      expr_index++;
      str_index++;

      if (expr_index == s_expr) {
        count++;
        expr_index = lps.array[s_expr - 1];
      }

      continue;
    }

    if (expr_index > 0) {
      expr_index = lps.array[expr_index - 1];
      continue;
    }

    str_index++;
  }

  vector_free(u64, &lps);
  return count;
}

u64 string8_count_words(const str8 source) {
  if (source.str == NULL)
    return 0;

  bool prev_is_space = __string8_char_is_whitespace(source.str[0]);

  u64 count = !prev_is_space;

  const u64 s_str = source.s_str;
  for (u64 i = 1; i < s_str; i++) {
    bool curr_is_space = __string8_char_is_whitespace(source.str[i]);
    count += prev_is_space && !curr_is_space;
    prev_is_space = curr_is_space;
  }

  return count;
}

// ============================# String8 Malloc #===============================

str8 string8_copy(const str8 source) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  u8* cpy = (u8*)malloc(source.s_str * sizeof(u8));
  if (cpy == NULL)
    return STR8_NULL;

  memcpy(cpy, source.str, source.s_str * sizeof(u8));

  return string8_create(source.s_str, cpy);
}

str8 string8_trim(const str8 source) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  const u64 s_str = source.s_str;

  u64 
    s_pad_start = 0,
    s_pad_end = 0;
  for (
    ;
    s_pad_start < s_str &&
    __string8_char_is_whitespace(source.str[s_pad_start]);
    s_pad_start++
  );

  if (s_pad_start < source.s_str)
    for (
      u64 i = source.s_str - 1;
      s_pad_end < s_str &&
      __string8_char_is_whitespace(source.str[i]);
      i--,
      s_pad_end++
    );

  const u64 s_cpy = s_str - s_pad_start - s_pad_end;
  u8* cpy = (u8*)malloc((s_cpy + 1) * sizeof(u8));
  if (cpy == NULL)
    return STR8_NULL;

  memcpy(cpy, &source.str[s_pad_start], s_cpy * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  return string8_create(s_cpy, cpy);
}

str8 string8_strip(const str8 source) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;
  
  const u64 s_str = source.s_str;

  vector(u64) matches = vector_init(u64, 0, 0);
  for (u64 i = 0; i < source.s_str; i++)
    if (__string8_char_is_whitespace(source.str[i]))
      vector_append(u64, &matches, i);

  const u64 s_matches = matches.s_vector;
  
  const u64 s_cpy = s_str - s_matches;
  u8* cpy = (u8*)malloc((s_cpy + 1) * sizeof(u8));
  if (cpy == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }
  
  u64
    write_pos = 0,
    read_pos = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;

    memcpy(&cpy[write_pos], &source.str[read_pos], copy_len * sizeof(u8));

    write_pos += copy_len;
    read_pos = match_start + 1; // sizeof(whitespace)
  }
  
  memcpy(&cpy[write_pos], &source.str[read_pos], (s_str - read_pos) * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  vector_free(u64, &matches);
  return string8_create(s_cpy, cpy);
}

str8 string8_remove(const str8 source, const str8 expr) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;
  
  const u64
    s_str = source.s_str,
    s_expr = expr.s_str;

  vector(u64) matches = string8_indexes_of(source, expr);
  const u64 s_matches = matches.s_vector;
  
  const u64 s_cpy = s_str - (s_matches * s_expr);
  u8* cpy = (u8*)malloc((s_cpy + 1) * sizeof(u8));
  if (cpy == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }
  
  u64
    write_pos = 0,
    read_pos = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;

    memcpy(&cpy[write_pos], &source.str[read_pos], copy_len * sizeof(u8));

    write_pos += copy_len;
    read_pos = match_start + s_expr;
  }
  
  memcpy(&cpy[write_pos], &source.str[read_pos], (s_str - read_pos) * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  vector_free(u64, &matches);
  return string8_create(s_cpy, cpy);
}

vector(str8) string8_split(const str8 source, const str8 expr) {
  if (source.s_str == 0 || source.str == NULL)
    return VECTOR_NULL(str8);
  
  const u64 
    s_str = source.s_str,
    s_expr = expr.s_str;
  
  vector(u64) matches = string8_indexes_of(source, expr);
  const u64 s_matches = matches.s_vector;
  
  vector(str8) str_array = vector_init(str8, s_matches + 1, STR8_NULL);
  
  u64
    read_pos = 0,
    s_str_array = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;
    
    u8* str_copy = (u8*)malloc((copy_len + 1) * sizeof(u8));
    if (str_copy == NULL) {
      vector_free(str8, &str_array);
      vector_free(u64, &matches);
      return VECTOR_NULL(str8);
    }
    
    memcpy(str_copy, &source.str[read_pos], copy_len * sizeof(u8));
    str_copy[copy_len] = CHAR_EPSI;

    str_array.array[s_str_array].str = str_copy;
    str_array.array[s_str_array].s_str = copy_len;
    s_str_array++;
    
    read_pos = match_start + s_expr;
  }
  
  const u64 final_len = s_str - read_pos;
  if (final_len > 0) {
    u8* str_copy = (u8*)malloc((final_len + 1) * sizeof(u8));
    if (str_copy == NULL) {
      vector_free(str8, &str_array);
      vector_free(u64, &matches);
      return VECTOR_NULL(str8);
    }
    
    memcpy(str_copy, &source.str[read_pos], final_len * sizeof(u8));
    str_copy[final_len] = CHAR_EPSI;

    str_array.array[s_str_array].str = str_copy;
    str_array.array[s_str_array].s_str = final_len;
    s_str_array++;
  }
  
  str_array.s_vector = s_str_array;
  vector_shrink_to_fit(str8, &str_array);
  vector_free(u64, &matches);
  
  return str_array;
}

str8 string8_join(const vector(str8) source) {
  if (source.s_vector == 0)
    return STR8_NULL;

  const u64 s_vector = source.s_vector;

  u64 s_str = 0;
  for (
    u64 i = 0;
    i < s_vector;
    s_str += source.array[i].s_str,
    i++
  );

  if (s_str == 0)
    return STR8_NULL;

  u8* joined_str = (u8*)malloc((s_str + 1) * sizeof(u8));
  if (joined_str == NULL)
    return STR8_NULL;

  u64 index = 0;
  for (u64 i = 0; i < s_vector; i++) {
    const u64 s_ith_str = source.array[i].s_str;
    if (s_str == 0)
      continue;

    memcpy(&joined_str[index], source.array[i].str, s_ith_str * sizeof(u8));
    index += s_ith_str;
  }
  joined_str[index] = CHAR_EPSI;

  return string8_create(s_str, joined_str);
}

str8 string8_replace(const str8 source, const str8 find_expr, const str8 replace_expr) {
  if (
    source.s_str == 0  || find_expr.s_str == 0  || replace_expr.s_str == 0 ||
    source.str == NULL || find_expr.str == NULL || replace_expr.str == NULL
  )
    return STR8_NULL;

  vector(u64) matches = string8_indexes_of(source, find_expr);
  const u64 s_matches = matches.s_vector;

  if (s_matches == 0)
    return string8_copy(source);

  u64 
    s_replaced_str = 0,
    read_index     = 0;

  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_index)
      continue;
    
    s_replaced_str += (match_start - read_index) + replace_expr.s_str;
    read_index = match_start + find_expr.s_str;
  }
  s_replaced_str += source.s_str - read_index;

  u8* replaced_str = (u8*)malloc((s_replaced_str + 1) * sizeof(u8));
  if (replaced_str == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }

  u64 
    read_pos  = 0,
    write_pos = 0;

  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;

    const u64 s_non_match = match_start - read_pos;

    memcpy(&replaced_str[write_pos], &source.str[read_pos], s_non_match * sizeof(u8));
    write_pos += s_non_match;

    memcpy(&replaced_str[write_pos], replace_expr.str, replace_expr.s_str * sizeof(u8));
    write_pos += replace_expr.s_str;

    read_pos = match_start + find_expr.s_str;
  }

  memcpy(&replaced_str[write_pos], &source.str[read_pos], (source.s_str - read_pos) * sizeof(u8));
  replaced_str[s_replaced_str] = CHAR_EPSI;

  vector_free(u64, &matches);

  return string8_create(s_replaced_str, replaced_str);
}

str8 string8_repeat(const str8 source, const u64 n) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  const u64 s_repeated_str = n * source.s_str;
  u8* repeated_str = (u8*)malloc((s_repeated_str + 1) * sizeof(u8));
  if (repeated_str == NULL)
    return STR8_NULL;

  for (u64 i = 0; i < n; i++)
    memcpy(&repeated_str[i * source.s_str], source.str, source.s_str * sizeof(u8));
  repeated_str[s_repeated_str] = CHAR_EPSI;

  return string8_create(s_repeated_str, repeated_str);
}

str8 string8_truncate(const str8 source, const u64 pos) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pos == 0          || pos >= source.s_str
  )
    return STR8_NULL;

  u8* truncated_str = (u8*)malloc((pos + 1) * sizeof(u8));
  if (truncated_str == NULL)
    return STR8_NULL;

  memcpy(truncated_str, source.str, pos * sizeof(u8));
  truncated_str[pos] = CHAR_EPSI;

  return string8_create(pos, truncated_str);
}

str8 string8_pad_start(const str8 source, const str8 pad) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pad.s_str    == 0 || pad.str == NULL
  )
    return STR8_NULL;

  const u64 s_padded_str = pad.s_str + source.s_str;

  u8* padded_str = (u8*)malloc((s_padded_str + 1) * sizeof(u8));
  if (padded_str == NULL)
    return STR8_NULL;

  memcpy(padded_str, pad.str, pad.s_str * sizeof(u8));
  memcpy(&padded_str[pad.s_str], source.str, source.s_str * sizeof(u8));
  padded_str[s_padded_str] = CHAR_EPSI;

  return string8_create(s_padded_str, padded_str);
}

str8 string8_pad_end(const str8 source, const str8 pad) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pad.s_str    == 0 || pad.str    == NULL
  )
    return STR8_NULL;

  const u64 s_padded_str = pad.s_str + source.s_str;

  u8* padded_str = (u8*)malloc((s_padded_str + 1) * sizeof(u8));
  if (padded_str == NULL)
    return STR8_NULL;

  memcpy(padded_str, source.str, source.s_str * sizeof(u8));
  memcpy(&padded_str[source.s_str], pad.str, pad.s_str * sizeof(u8));
  padded_str[s_padded_str] = CHAR_EPSI;

  return string8_create(s_padded_str, padded_str);
}

str8 string8_substring(const str8 source, const u64 start, const u64 end) {
  if (
    source.s_str == 0 || source.str == NULL ||
    start > end       || end > source.s_str
  )
    return STR8_NULL;

  const u64 s_substring = end - start;

  u8* substring = (u8*)malloc((s_substring + 1) * sizeof(u8));
  if (substring == NULL)
    return STR8_NULL;

  memcpy(substring, &source.str[start], s_substring * sizeof(u8));
  substring[s_substring] = CHAR_EPSI;

  return string8_create(s_substring, substring);
}

str8 string8_format(const char* fmt, ...) {
  str8 result = {0};
  
  va_list args;
  va_start(args, fmt);
  
  va_list args_copy;

  // Copy because vsnprintf consumes the va_list
  va_copy(args_copy, args);
  i32 size = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);
  
  if (size < 0) {
    va_end(args);
    return STR8_NULL;
  }
  
  result.str = (u8*)malloc((size + 1) * sizeof(u8));
  if (result.str == NULL) {
    va_end(args);
    return STR8_NULL;
  }
  
  vsnprintf((char*)result.str, size + 1, fmt, args);
  result.s_str = size;
  
  va_end(args);
  return result;
}

str8 string8_vformat(const char* fmt, va_list args) {
  str8 result = {0};
  
  va_list args_copy;

  // Copy because vsnprintf consumes the va_list
  va_copy(args_copy, args);
  i32 size = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);
  
  if (size < 0)
    return STR8_NULL;
  
  result.str = (u8*)malloc((size + 1) * sizeof(u8));
  if (result.str == NULL)
    return STR8_NULL;
  
  vsnprintf((char*)result.str, size + 1, fmt, args);
  result.s_str = size;
  
  return result;
}

str8 string8_read_file(const char* filename) {
  if (filename == NULL)
    return STR8_NULL;

  FILE* file = fopen(filename, "rb");
  if (file == NULL)
    return STR8_NULL;

  fseek(file, 0, SEEK_END);
  const u64 s_file = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (s_file == 0) {
    fclose(file);
    return STR8_NULL;
  }

  u8* file_str = (u8*)malloc(s_file * sizeof(u8));
  if (file_str == NULL) {
    fclose(file);
    return STR8_NULL;
  }

  str8 result = {
    .s_str = fread(file_str, 1, s_file, file),
    .str   = file_str
  };

  fclose(file);
  return result;
}

// ============================# String8 Arena Alloc #===============================

str8 string8_copy_arena(const str8 source, Arena* arena) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  u8* cpy = (u8*)arena_alloc(arena, source.s_str * sizeof(u8));
  if (cpy == NULL)
    return STR8_NULL;

  memcpy(cpy, source.str, source.s_str * sizeof(u8));

  return string8_create(source.s_str, cpy);
}

str8 string8_trim_arena(const str8 source, Arena* arena) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  const u64 s_str = source.s_str;

  u64 
    s_pad_start = 0,
    s_pad_end = 0;
  for (
    ;
    s_pad_start < s_str &&
    __string8_char_is_whitespace(source.str[s_pad_start]);
    s_pad_start++
  );

  if (s_pad_start < source.s_str)
    for (
      u64 i = source.s_str - 1;
      s_pad_end < s_str &&
      __string8_char_is_whitespace(source.str[i]);
      i--,
      s_pad_end++
    );

  const u64 s_cpy = s_str - s_pad_start - s_pad_end;
  u8* cpy = (u8*)arena_alloc(arena, (s_cpy + 1) * sizeof(u8));
  if (cpy == NULL)
    return STR8_NULL;

  memcpy(cpy, &source.str[s_pad_start], s_cpy * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  return string8_create(s_cpy, cpy);
}

str8 string8_strip_arena(const str8 source, Arena* arena) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;
  
  const u64 s_str = source.s_str;

  vector(u64) matches = vector_init(u64, 0, 0);
  for (u64 i = 0; i < source.s_str; i++)
    if (__string8_char_is_whitespace(source.str[i]))
      vector_append(u64, &matches, i);

  const u64 s_matches = matches.s_vector;
  
  const u64 s_cpy = s_str - s_matches;
  u8* cpy = (u8*)arena_alloc(arena, (s_cpy + 1) * sizeof(u8));
  if (cpy == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }
  
  u64
    write_pos = 0,
    read_pos = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;

    memcpy(&cpy[write_pos], &source.str[read_pos], copy_len * sizeof(u8));

    write_pos += copy_len;
    read_pos = match_start + 1; // sizeof(whitespace)
  }
  
  memcpy(&cpy[write_pos], &source.str[read_pos], (s_str - read_pos) * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  vector_free(u64, &matches);
  return string8_create(s_cpy, cpy);
}

str8 string8_remove_arena(const str8 source, const str8 expr, Arena* arena) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;
  
  const u64
    s_str = source.s_str,
    s_expr = expr.s_str;

  vector(u64) matches = string8_indexes_of(source, expr);
  const u64 s_matches = matches.s_vector;
  
  const u64 s_cpy = s_str - (s_matches * s_expr);
  u8* cpy = (u8*)arena_alloc(arena, (s_cpy + 1) * sizeof(u8));
  if (cpy == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }
  
  u64
    write_pos = 0,
    read_pos = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;

    memcpy(&cpy[write_pos], &source.str[read_pos], copy_len * sizeof(u8));

    write_pos += copy_len;
    read_pos = match_start + s_expr;
  }
  
  memcpy(&cpy[write_pos], &source.str[read_pos], (s_str - read_pos) * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  vector_free(u64, &matches);
  return string8_create(s_cpy, cpy);
}

vector(str8) string8_split_arena(const str8 source, const str8 expr, Arena* arena) {
  if (source.s_str == 0 || source.str == NULL)
    return VECTOR_NULL(str8);
  
  const u64 
    s_str = source.s_str,
    s_expr = expr.s_str;
  
  vector(u64) matches = string8_indexes_of(source, expr);
  const u64 s_matches = matches.s_vector;
  
  vector(str8) str_array = vector_init(str8, s_matches + 1, STR8_NULL);
  
  u64
    read_pos = 0,
    s_str_array = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;
    
    u8* str_copy = (u8*)arena_alloc(arena, (copy_len + 1) * sizeof(u8));
    if (str_copy == NULL) {
      vector_free(str8, &str_array);
      vector_free(u64, &matches);
      return VECTOR_NULL(str8);
    }
    
    memcpy(str_copy, &source.str[read_pos], copy_len * sizeof(u8));
    str_copy[copy_len] = CHAR_EPSI;

    str_array.array[s_str_array].str = str_copy;
    str_array.array[s_str_array].s_str = copy_len;
    s_str_array++;
    
    read_pos = match_start + s_expr;
  }
  
  const u64 final_len = s_str - read_pos;
  if (final_len > 0) {
    u8* str_copy = (u8*)arena_alloc(arena, (final_len + 1) * sizeof(u8));
    if (str_copy == NULL) {
      vector_free(str8, &str_array);
      vector_free(u64, &matches);
      return VECTOR_NULL(str8);
    }
    
    memcpy(str_copy, &source.str[read_pos], final_len * sizeof(u8));
    str_copy[final_len] = CHAR_EPSI;

    str_array.array[s_str_array].str = str_copy;
    str_array.array[s_str_array].s_str = final_len;
    s_str_array++;
  }
  
  str_array.s_vector = s_str_array;
  vector_shrink_to_fit(str8, &str_array);
  vector_free(u64, &matches);
  
  return str_array;
}

str8 string8_join_arena(const vector(str8) source, Arena* arena) {
  if (source.s_vector == 0)
    return STR8_NULL;

  const u64 s_vector = source.s_vector;

  u64 s_str = 0;
  for (
    u64 i = 0;
    i < s_vector;
    s_str += source.array[i].s_str,
    i++
  );

  if (s_str == 0)
    return STR8_NULL;

  u8* joined_str = (u8*)arena_alloc(arena, (s_str + 1) * sizeof(u8));
  if (joined_str == NULL)
    return STR8_NULL;

  u64 index = 0;
  for (u64 i = 0; i < s_vector; i++) {
    const u64 s_ith_str = source.array[i].s_str;
    if (s_str == 0)
      continue;

    memcpy(&joined_str[index], source.array[i].str, s_ith_str * sizeof(u8));
    index += s_ith_str;
  }
  joined_str[index] = CHAR_EPSI;

  return string8_create(s_str, joined_str);
}

str8 string8_replace_arena(const str8 source, const str8 find_expr, const str8 replace_expr, Arena* arena) {
  if (
    source.s_str == 0  || find_expr.s_str == 0  || replace_expr.s_str == 0 ||
    source.str == NULL || find_expr.str == NULL || replace_expr.str == NULL
  )
    return STR8_NULL;

  vector(u64) matches = string8_indexes_of(source, find_expr);
  const u64 s_matches = matches.s_vector;

  if (s_matches == 0)
    return string8_copy(source);

  u64 
    s_replaced_str = 0,
    read_index     = 0;

  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_index)
      continue;
    
    s_replaced_str += (match_start - read_index) + replace_expr.s_str;
    read_index = match_start + find_expr.s_str;
  }
  s_replaced_str += source.s_str - read_index;

  u8* replaced_str = (u8*)arena_alloc(arena, (s_replaced_str + 1) * sizeof(u8));
  if (replaced_str == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }

  u64 
    read_pos  = 0,
    write_pos = 0;

  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;

    const u64 s_non_match = match_start - read_pos;

    memcpy(&replaced_str[write_pos], &source.str[read_pos], s_non_match * sizeof(u8));
    write_pos += s_non_match;

    memcpy(&replaced_str[write_pos], replace_expr.str, replace_expr.s_str * sizeof(u8));
    write_pos += replace_expr.s_str;

    read_pos = match_start + find_expr.s_str;
  }

  memcpy(&replaced_str[write_pos], &source.str[read_pos], (source.s_str - read_pos) * sizeof(u8));
  replaced_str[s_replaced_str] = CHAR_EPSI;

  vector_free(u64, &matches);

  return string8_create(s_replaced_str, replaced_str);
}

str8 string8_repeat_arena(const str8 source, const u64 n, Arena* arena) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  const u64 s_repeated_str = n * source.s_str;
  u8* repeated_str = (u8*)arena_alloc(arena, (s_repeated_str + 1) * sizeof(u8));
  if (repeated_str == NULL)
    return STR8_NULL;

  for (u64 i = 0; i < n; i++)
    memcpy(&repeated_str[i * source.s_str], source.str, source.s_str * sizeof(u8));
  repeated_str[s_repeated_str] = CHAR_EPSI;

  return string8_create(s_repeated_str, repeated_str);
}

str8 string8_truncate_arena(const str8 source, const u64 pos, Arena* arena) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pos == 0          || pos >= source.s_str
  )
    return STR8_NULL;

  u8* truncated_str = (u8*)arena_alloc(arena, (pos + 1) * sizeof(u8));
  if (truncated_str == NULL)
    return STR8_NULL;

  memcpy(truncated_str, source.str, pos * sizeof(u8));
  truncated_str[pos] = CHAR_EPSI;

  return string8_create(pos, truncated_str);
}

str8 string8_pad_start_arena(const str8 source, const str8 pad, Arena* arena) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pad.s_str    == 0 || pad.str    == NULL
  )
    return STR8_NULL;

  const u64 s_padded_str = pad.s_str + source.s_str;

  u8* padded_str = (u8*)arena_alloc(arena, (s_padded_str + 1) * sizeof(u8));
  if (padded_str == NULL)
    return STR8_NULL;

  memcpy(padded_str, pad.str, pad.s_str * sizeof(u8));
  memcpy(&padded_str[pad.s_str], source.str, source.s_str * sizeof(u8));
  padded_str[s_padded_str] = CHAR_EPSI;

  return string8_create(s_padded_str, padded_str);
}

str8 string8_pad_end_arena(const str8 source, const str8 pad, Arena* arena) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pad.s_str    == 0 || pad.str    == NULL
  )
    return STR8_NULL;

  const u64 s_padded_str = pad.s_str + source.s_str;

  u8* padded_str = (u8*)arena_alloc(arena, (s_padded_str + 1) * sizeof(u8));
  if (padded_str == NULL)
    return STR8_NULL;

  memcpy(padded_str, source.str, source.s_str * sizeof(u8));
  memcpy(&padded_str[source.s_str], pad.str, pad.s_str * sizeof(u8));
  padded_str[s_padded_str] = CHAR_EPSI;

  return string8_create(s_padded_str, padded_str);
}

str8 string8_substring_arena(const str8 source, const u64 start, const u64 end, Arena* arena) {
  if (
    source.s_str == 0 || source.str == NULL ||
    start > end       || end > source.s_str
  )
    return STR8_NULL;

  const u64 s_substring = end - start;

  u8* substring = (u8*)arena_alloc(arena, (s_substring + 1) * sizeof(u8));
  if (substring == NULL)
    return STR8_NULL;

  memcpy(substring, &source.str[start], s_substring * sizeof(u8));
  substring[s_substring] = CHAR_EPSI;

  return string8_create(s_substring, substring);
}

str8 string8_format_arena(Arena* arena, const char* fmt, ...) {
  str8 result = {0};
  
  va_list args;
  va_start(args, fmt);
  
  va_list args_copy;

  // Copy because vsnprintf consumes the va_list
  va_copy(args_copy, args);
  i32 size = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);
  
  if (size < 0) {
    va_end(args);
    return STR8_NULL;
  }
  
  result.str = (u8*)arena_alloc(arena, (size + 1) * sizeof(u8));
  if (result.str == NULL) {
    va_end(args);
    return STR8_NULL;
  }
  
  vsnprintf((char*)result.str, size + 1, fmt, args);
  result.s_str = size;
  
  va_end(args);
  return result;
}

str8 string8_vformat_arena(Arena* arena, const char* fmt, va_list args) {
  str8 result = {0};
  
  va_list args_copy;

  // Copy because vsnprintf consumes the va_list
  va_copy(args_copy, args);
  i32 size = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);
  
  if (size < 0)
    return STR8_NULL;
  
  result.str = (u8*)arena_alloc(arena, (size + 1) * sizeof(u8));
  if (result.str == NULL)
    return STR8_NULL;
  
  vsnprintf((char*)result.str, size + 1, fmt, args);
  result.s_str = size;
  
  return result;
}

str8 string8_read_file_arena(const char* filename, Arena* arena) {
  if (filename == NULL)
    return STR8_NULL;

  FILE* file = fopen(filename, "rb");
  if (file == NULL)
    return STR8_NULL;

  fseek(file, 0, SEEK_END);
  const u64 s_file = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (s_file == 0) {
    fclose(file);
    return STR8_NULL;
  }

  u8* file_str = (u8*)arena_alloc(arena, s_file * sizeof(u8));
  if (file_str == NULL) {
    fclose(file);
    return STR8_NULL;
  }

  str8 result = {
    .s_str = fread(file_str, 1, s_file, file),
    .str   = file_str
  };

  fclose(file);
  return result;
}

// ============================# String8 Pool Alloc #===============================

str8 string8_copy_pool(const str8 source, Pool* pool) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  u8* cpy = (u8*)pool_alloc(pool, source.s_str * sizeof(u8));
  if (cpy == NULL)
    return STR8_NULL;

  memcpy(cpy, source.str, source.s_str * sizeof(u8));

  return string8_create(source.s_str, cpy);
}

str8 string8_trim_pool(const str8 source, Pool* pool) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  const u64 s_str = source.s_str;

  u64 
    s_pad_start = 0,
    s_pad_end = 0;
  for (
    ;
    s_pad_start < s_str &&
    __string8_char_is_whitespace(source.str[s_pad_start]);
    s_pad_start++
  );

  if (s_pad_start < source.s_str)
    for (
      u64 i = source.s_str - 1;
      s_pad_end < s_str &&
      __string8_char_is_whitespace(source.str[i]);
      i--,
      s_pad_end++
    );

  const u64 s_cpy = s_str - s_pad_start - s_pad_end;
  u8* cpy = (u8*)pool_alloc(pool, (s_cpy + 1) * sizeof(u8));
  if (cpy == NULL)
    return STR8_NULL;

  memcpy(cpy, &source.str[s_pad_start], s_cpy * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  return string8_create(s_cpy, cpy);
}

str8 string8_strip_pool(const str8 source, Pool* pool) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;
  
  const u64 s_str = source.s_str;

  vector(u64) matches = vector_init(u64, 0, 0);
  for (u64 i = 0; i < source.s_str; i++)
    if (__string8_char_is_whitespace(source.str[i]))
      vector_append(u64, &matches, i);

  const u64 s_matches = matches.s_vector;
  
  const u64 s_cpy = s_str - s_matches;
  u8* cpy = (u8*)pool_alloc(pool, (s_cpy + 1) * sizeof(u8));
  if (cpy == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }
  
  u64
    write_pos = 0,
    read_pos = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;

    memcpy(&cpy[write_pos], &source.str[read_pos], copy_len * sizeof(u8));

    write_pos += copy_len;
    read_pos = match_start + 1; // sizeof(whitespace)
  }
  
  memcpy(&cpy[write_pos], &source.str[read_pos], (s_str - read_pos) * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  vector_free(u64, &matches);
  return string8_create(s_cpy, cpy);
}

str8 string8_remove_pool(const str8 source, const str8 expr, Pool* pool) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;
  
  const u64
    s_str = source.s_str,
    s_expr = expr.s_str;

  vector(u64) matches = string8_indexes_of(source, expr);
  const u64 s_matches = matches.s_vector;
  
  const u64 s_cpy = s_str - (s_matches * s_expr);
  u8* cpy = (u8*)pool_alloc(pool, (s_cpy + 1) * sizeof(u8));
  if (cpy == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }
  
  u64
    write_pos = 0,
    read_pos = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;

    memcpy(&cpy[write_pos], &source.str[read_pos], copy_len * sizeof(u8));

    write_pos += copy_len;
    read_pos = match_start + s_expr;
  }
  
  memcpy(&cpy[write_pos], &source.str[read_pos], (s_str - read_pos) * sizeof(u8));
  cpy[s_cpy] = CHAR_EPSI;
  
  vector_free(u64, &matches);
  return string8_create(s_cpy, cpy);
}

vector(str8) string8_split_pool(const str8 source, const str8 expr, Pool* pool) {
  if (source.s_str == 0 || source.str == NULL)
    return VECTOR_NULL(str8);
  
  const u64 
    s_str = source.s_str,
    s_expr = expr.s_str;
  
  vector(u64) matches = string8_indexes_of(source, expr);
  const u64 s_matches = matches.s_vector;
  
  vector(str8) str_array = vector_init(str8, s_matches + 1, STR8_NULL);
  
  u64
    read_pos = 0,
    s_str_array = 0;
  
  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;
    
    const u64 copy_len = match_start - read_pos;
    
    u8* str_copy = (u8*)pool_alloc(pool, (copy_len + 1) * sizeof(u8));
    if (str_copy == NULL) {
      vector_free(str8, &str_array);
      vector_free(u64, &matches);
      return VECTOR_NULL(str8);
    }
    
    memcpy(str_copy, &source.str[read_pos], copy_len * sizeof(u8));
    str_copy[copy_len] = CHAR_EPSI;

    str_array.array[s_str_array].str = str_copy;
    str_array.array[s_str_array].s_str = copy_len;
    s_str_array++;
    
    read_pos = match_start + s_expr;
  }
  
  const u64 final_len = s_str - read_pos;
  if (final_len > 0) {
    u8* str_copy = (u8*)pool_alloc(pool, (final_len + 1) * sizeof(u8));
    if (str_copy == NULL) {
      vector_free(str8, &str_array);
      vector_free(u64, &matches);
      return VECTOR_NULL(str8);
    }
    
    memcpy(str_copy, &source.str[read_pos], final_len * sizeof(u8));
    str_copy[final_len] = CHAR_EPSI;

    str_array.array[s_str_array].str = str_copy;
    str_array.array[s_str_array].s_str = final_len;
    s_str_array++;
  }
  
  str_array.s_vector = s_str_array;
  vector_shrink_to_fit(str8, &str_array);
  vector_free(u64, &matches);
  
  return str_array;
}

str8 string8_join_pool(const vector(str8) source, Pool* pool) {
  if (source.s_vector == 0)
    return STR8_NULL;

  const u64 s_vector = source.s_vector;

  u64 s_str = 0;
  for (
    u64 i = 0;
    i < s_vector;
    s_str += source.array[i].s_str,
    i++
  );

  if (s_str == 0)
    return STR8_NULL;

  u8* joined_str = (u8*)pool_alloc(pool, (s_str + 1) * sizeof(u8));
  if (joined_str == NULL)
    return STR8_NULL;

  u64 index = 0;
  for (u64 i = 0; i < s_vector; i++) {
    const u64 s_ith_str = source.array[i].s_str;
    if (s_str == 0)
      continue;

    memcpy(&joined_str[index], source.array[i].str, s_ith_str * sizeof(u8));
    index += s_ith_str;
  }
  joined_str[index] = CHAR_EPSI;

  return string8_create(s_str, joined_str);
}

str8 string8_replace_pool(const str8 source, const str8 find_expr, const str8 replace_expr, Pool* pool) {
  if (
    source.s_str == 0  || find_expr.s_str == 0  || replace_expr.s_str == 0 ||
    source.str == NULL || find_expr.str == NULL || replace_expr.str == NULL
  )
    return STR8_NULL;

  vector(u64) matches = string8_indexes_of(source, find_expr);
  const u64 s_matches = matches.s_vector;

  if (s_matches == 0)
    return string8_copy(source);

  u64 
    s_replaced_str = 0,
    read_index     = 0;

  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_index)
      continue;
    
    s_replaced_str += (match_start - read_index) + replace_expr.s_str;
    read_index = match_start + find_expr.s_str;
  }
  s_replaced_str += source.s_str - read_index;

  u8* replaced_str = (u8*)pool_alloc(pool, (s_replaced_str + 1) * sizeof(u8));
  if (replaced_str == NULL) {
    vector_free(u64, &matches);
    return STR8_NULL;
  }

  u64 
    read_pos  = 0,
    write_pos = 0;

  for (u64 i = 0; i < s_matches; i++) {
    const u64 match_start = matches.array[i];
    if (match_start < read_pos)
      continue;

    const u64 s_non_match = match_start - read_pos;

    memcpy(&replaced_str[write_pos], &source.str[read_pos], s_non_match * sizeof(u8));
    write_pos += s_non_match;

    memcpy(&replaced_str[write_pos], replace_expr.str, replace_expr.s_str * sizeof(u8));
    write_pos += replace_expr.s_str;

    read_pos = match_start + find_expr.s_str;
  }

  memcpy(&replaced_str[write_pos], &source.str[read_pos], (source.s_str - read_pos) * sizeof(u8));
  replaced_str[s_replaced_str] = CHAR_EPSI;

  vector_free(u64, &matches);

  return string8_create(s_replaced_str, replaced_str);
}

str8 string8_repeat_pool(const str8 source, const u64 n, Pool* pool) {
  if (source.s_str == 0 || source.str == NULL)
    return STR8_NULL;

  const u64 s_repeated_str = n * source.s_str;
  u8* repeated_str = (u8*)pool_alloc(pool, (s_repeated_str + 1) * sizeof(u8));
  if (repeated_str == NULL)
    return STR8_NULL;

  for (u64 i = 0; i < n; i++)
    memcpy(&repeated_str[i * source.s_str], source.str, source.s_str * sizeof(u8));
  repeated_str[s_repeated_str] = CHAR_EPSI;

  return string8_create(s_repeated_str, repeated_str);
}

str8 string8_truncate_pool(const str8 source, const u64 pos, Pool* pool) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pos == 0          || pos >= source.s_str
  )
    return STR8_NULL;

  u8* truncated_str = (u8*)pool_alloc(pool, (pos + 1) * sizeof(u8));
  if (truncated_str == NULL)
    return STR8_NULL;

  memcpy(truncated_str, source.str, pos * sizeof(u8));
  truncated_str[pos] = CHAR_EPSI;

  return string8_create(pos, truncated_str);
}

str8 string8_pad_start_pool(const str8 source, const str8 pad, Pool* pool) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pad.s_str    == 0 || pad.str    == NULL
  )
    return STR8_NULL;

  const u64 s_padded_str = pad.s_str + source.s_str;

  u8* padded_str = (u8*)pool_alloc(pool, s_padded_str * sizeof(u8));
  if (padded_str == NULL)
    return STR8_NULL;

  memcpy(padded_str, pad.str, pad.s_str * sizeof(u8));
  memcpy(&padded_str[pad.s_str], source.str, source.s_str * sizeof(u8));

  return string8_create(s_padded_str, padded_str);
}

str8 string8_pad_end_pool(const str8 source, const str8 pad, Pool* pool) {
  if (
    source.s_str == 0 || source.str == NULL ||
    pad.s_str    == 0 || pad.str    == NULL
  )
    return STR8_NULL;

  const u64 s_padded_str = pad.s_str + source.s_str;

  u8* padded_str = (u8*)pool_alloc(pool, (s_padded_str + 1) * sizeof(u8));
  if (padded_str == NULL)
    return STR8_NULL;

  memcpy(padded_str, source.str, source.s_str * sizeof(u8));
  memcpy(&padded_str[source.s_str], pad.str, pad.s_str * sizeof(u8));
  padded_str[s_padded_str] = CHAR_EPSI;

  return string8_create(s_padded_str, padded_str);
}

str8 string8_substring_pool(const str8 source, const u64 start, const u64 end, Pool* pool) {
  if (
    source.s_str == 0 || source.str == NULL ||
    start > end       || end > source.s_str
  )
    return STR8_NULL;

  const u64 s_substring = end - start;

  u8* substring = (u8*)pool_alloc(pool, (s_substring + 1) * sizeof(u8));
  if (substring == NULL)
    return STR8_NULL;

  memcpy(substring, &source.str[start], s_substring * sizeof(u8));
  substring[s_substring] = CHAR_EPSI;

  return string8_create(s_substring, substring);
}

str8 string8_format_pool(Pool* pool, const char* fmt, ...) {
  str8 result = {0};
  
  va_list args;
  va_start(args, fmt);
  
  va_list args_copy;

  // Copy because vsnprintf consumes the va_list
  va_copy(args_copy, args);
  i32 size = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);
  
  if (size < 0) {
    va_end(args);
    return STR8_NULL;
  }
  
  result.str = (u8*)pool_alloc(pool, (size + 1) * sizeof(u8));
  if (result.str == NULL) {
    va_end(args);
    return STR8_NULL;
  }
  
  vsnprintf((char*)result.str, size + 1, fmt, args);
  result.s_str = size;
  
  va_end(args);
  return result;
}

str8 string8_vformat_pool(Pool* pool, const char* fmt, va_list args) {
  str8 result = {0};
  
  va_list args_copy;

  // Copy because vsnprintf consumes the va_list
  va_copy(args_copy, args);
  i32 size = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);
  
  if (size < 0)
    return STR8_NULL;
  
  result.str = (u8*)pool_alloc(pool, (size + 1) * sizeof(u8));
  if (result.str == NULL)
    return STR8_NULL;
  
  vsnprintf((char*)result.str, size + 1, fmt, args);
  result.s_str = size;
  
  return result;
}

str8 string8_read_file_pool(const char* filename, Pool* pool) {
  if (filename == NULL)
    return STR8_NULL;

  FILE* file = fopen(filename, "rb");
  if (file == NULL)
    return STR8_NULL;

  fseek(file, 0, SEEK_END);
  const u64 s_file = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (s_file == 0) {
    fclose(file);
    return STR8_NULL;
  }

  u8* file_str = (u8*)pool_alloc(pool, s_file * sizeof(u8));
  if (file_str == NULL) {
    fclose(file);
    return STR8_NULL;
  }

  str8 result = {
    .s_str = fread(file_str, 1, s_file, file),
    .str   = file_str
  };

  fclose(file);
  return result;
}

// ============================# PRIVATE #===============================

vector(u64) __string8_build_kmp_lps(const str8 pattern) {
  const u64 s_pattern = pattern.s_str;

  vector(u64) lps = vector_init(u64, s_pattern, 0);
  if (vector_is_empty(u64, &lps))
    return VECTOR_NULL(u64);

  u64 len = 0;
  for (u32 i = 1; i < s_pattern;) {
    if (pattern.str[i] == pattern.str[len]) {
      len++;
      lps.array[i] = len;
      i++;
      continue;
    }

    if (len == 0) {
      lps.array[i] = 0;
      i++;
      continue;
    }

    len = lps.array[len - 1];
  }

  return lps;
}

inline bool __string8_char_is_alpha(const u8 c) {
  return 
    __string8_char_is_lower(c) ||
    __string8_char_is_upper(c);
}

inline bool __string8_char_is_lower(const u8 c) {
  return c >= CHAR_LOWER_A && c <= CHAR_LOWER_Z;
}

inline bool __string8_char_is_upper(const u8 c) {
  return c >= CHAR_UPPER_A && c <= CHAR_UPPER_Z;
}

inline bool __string8_char_is_hexa(const u8 c) {
  return
    __string8_char_is_num(c)                 ||
    (c >= CHAR_LOWER_A && c <= CHAR_LOWER_F) ||
    (c >= CHAR_UPPER_A && c <= CHAR_UPPER_F);
}

inline bool __string8_char_is_num(const u8 c) {
  return c >= CHAR_0 && c <= CHAR_9;
}

inline bool __string8_char_is_whitespace(const u8 c) {
  return
    c == CHAR_SPACE   ||
    c == CHAR_NEWLINE ||
    c == CHAR_TAB     ||
    c == CHAR_RET     ||
    c == CHAR_VTAB    ||
    c == CHAR_FFEED   ||
    c == CHAR_EPSI;
}

inline u8 __string8_char_to_lower(const u8 c) {
  return 
    __string8_char_is_upper(c) ? 
      (c - CHAR_UPPER_A + CHAR_LOWER_A) :
      c;
}

inline u8 __string8_char_to_upper(const u8 c) {
  return 
    __string8_char_is_lower(c) ? 
      (c - CHAR_LOWER_A + CHAR_UPPER_A) :
      c;
}

inline u64 __string8_utils_min(const u64 a, const u64 b) {
  return a <= b ? a : b;
}
