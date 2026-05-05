#ifndef clox_common_h
#define clox_common_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__has_c_attribute)
  #if __has_c_attribute(malloc)
    #define ALLOCATOR [[malloc]]
  #endif
#endif

#ifndef ALLOCATOR
  #if defined(__has_attribute)
    #if __has_attribute(malloc) && __has_attribute(warn_unused_result)
      #if defined(__GNUC__) && __GNUC__ >= 11
        #define ALLOCATOR __attribute__((malloc, malloc(free, 1), warn_unused_result))
      #else
        #define ALLOCATOR __attribute__((malloc, warn_unused_result))
      #endif
    #endif
  #endif
#endif

#ifndef ALLOCATOR
  #if defined(_MSC_VER)
    #define ALLOCATOR _Check_return_
  #else
    #define ALLOCATOR
  #endif
#endif

/* #define DEBUG_TRACE_EXECUTION */

#endif // clox_common_h
