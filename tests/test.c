

#ifndef C4_TYPES_H

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long      u64;

typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long        i64;

#else

#include <sys/types.h>
#include <stddef.h>

typedef u_int8_t           u8;
typedef u_int16_t          u16;
typedef u_int32_t          u32;
typedef u_int64_t          u64;

typedef int8_t             i8;
typedef int16_t            i16;
typedef int32_t            i32;
typedef int64_t            i64;

#endif

typedef float              f32;
typedef double             f64;

typedef u8 *   u8_ptr;
typedef u16 *  u16_ptr;
typedef u32 *  u32_ptr;
typedef u64 *  u64_ptr;

typedef i8 *   i8_ptr;
typedef i16 *  i16_ptr;
typedef i32 *  i32_ptr;
typedef i64 *  i64_ptr;

typedef f32 *  f32_ptr;
typedef f64 *  f64_ptr;

typedef void * void_ptr;

struct Color
{
	i32 red;
	i32 green;
	i32 blue;
};

i32 main()
{
	i32 age = 90;
	i32 *ptr1 = &age;
	i32 **ptr2 = &ptr1;
	struct Color color = (struct Color){
		.blue = 78,
		.green = 89,
		.red = 90,
	};

	return color.blue;
}
