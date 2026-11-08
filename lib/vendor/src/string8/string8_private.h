#ifndef __STRING8_PRIVATE_H__
#define __STRING8_PRIVATE_H__

#include <pair/pair.h>
#include "string8.h"

#define CHAR_UPPER_A 'A'    
#define CHAR_LOWER_A 'a'    
#define CHAR_UPPER_F 'F'
#define CHAR_LOWER_F 'f'
#define CHAR_UPPER_Z 'Z'    
#define CHAR_LOWER_Z 'z'

#define CHAR_0       '0'
#define CHAR_9       '9'

#define CHAR_SPACE   ' '
#define CHAR_NEWLINE '\n'
#define CHAR_TAB     '\t'
#define CHAR_RET     '\r'
#define CHAR_VTAB    '\v'
#define CHAR_FFEED   '\f'
#define CHAR_EPSI    '\0'

vector(u64) __string8_build_kmp_lps      (const str8 pattern);

inline bool __string8_char_is_alpha      (const u8 c);
inline bool __string8_char_is_lower      (const u8 c);
inline bool __string8_char_is_upper      (const u8 c);
inline bool __string8_char_is_hexa       (const u8 c);
inline bool __string8_char_is_num        (const u8 c);
inline bool __string8_char_is_whitespace (const u8 c);

inline u8   __string8_char_to_lower      (const u8 c);
inline u8   __string8_char_to_upper      (const u8 c);

inline u64  __string8_utils_min          (const u64 a, const u64 b);

#endif // !__STRING8_PRIVATE_H__
