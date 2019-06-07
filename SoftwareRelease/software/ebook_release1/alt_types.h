#ifndef ALT_TYPES_H_INCLUDED
#define ALT_TYPES_H_INCLUDED

#ifndef ALT_ASM_SRC
typedef signed char  alt_8;
typedef unsigned char  alt_u8;
typedef signed short alt_16;
typedef unsigned short alt_u16;
typedef signed long alt_32;
typedef unsigned long alt_u32;
typedef long long alt_64;
typedef unsigned long long alt_u64;
#endif

#define ALT_INLINE        __inline__
#define ALT_ALWAYS_INLINE __attribute__ ((always_inline))
#define ALT_WEAK          __attribute__((weak))


#endif // ALT_TYPES_H_INCLUDED
