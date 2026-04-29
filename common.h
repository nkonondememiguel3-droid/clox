#ifndef clox_common_h
#define clox_common_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__has_c_attribute) && __has_c_attribute(malloc)
  #define ALLOCATOR [[malloc(free)]]
#elif defined(__has_attribute) && __has_attribute(malloc) && __has_attribute(warn_unused_resutl)
  #if defined(__GNU__) && __GNUC__ >= 11
    #define ALLOCATOR __attribute__((malloc, malloc(free, 1), warn_unused_result))
  #else
    #define ALLOCATOR __attribute__((malloc, warn_unused_result))
  #endif
#elif defined(_MSC_VER)
  #define ALLOCATOR _Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(_Size)
#else 
  #define ALLOCATOR
#endif

/* #define DEBUG_TRACE_EXECUTION */

#endif // clox_common_h
