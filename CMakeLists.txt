cmake_minimum_required(VERSION 3.10)
project(clox)

option(ENABLE_DEBUG "Enable debug mode" ON)
option(ENABLE_SANITIZERS "Enable sanitizers" ON)
option(ENABLE_WERROR "Treat warnings as errors" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXTENSIONS OFF)

# Base debug flags (as a plain string)
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

# Optional debug trace macro
if(ENABLE_DEBUG)
    add_compile_definitions(DEBUG_TRACE_EXECUTION)
    add_compile_definitions(DEBUG_PRINT_CODE)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # Warnings – note space at the beginning of each APPEND
    string(APPEND CMAKE_C_FLAGS_DEBUG
        " -Wall -Wextra -Wpedantic"
        " -Wshadow -Wformat=2 -Wconversion"
        " -Wstrict-prototypes -Wmissing-prototypes"
        " -Wswitch-default"
    )

    if(ENABLE_WERROR)
        string(APPEND CMAKE_C_FLAGS_DEBUG " -Werror")
    endif()

    if(ENABLE_SANITIZERS)
        string(APPEND CMAKE_C_FLAGS_DEBUG
            " -fsanitize=address,undefined"
            " -fno-omit-frame-pointer"
            " -fno-optimize-sibling-calls"
        )
        string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG
            " -fsanitize=address,undefined"
        )
    endif()

    string(APPEND CMAKE_C_FLAGS_DEBUG " -gdwarf-4 -g3")

elseif(MSVC)
    string(APPEND CMAKE_C_FLAGS_DEBUG " /Zi /Od /RTC1 /W4")
    if(ENABLE_WERROR)
        string(APPEND CMAKE_C_FLAGS_DEBUG " /WX")
    endif()
endif()

# Make Debug the default build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(${PROJECT_NAME}
    main.c
    memory.c memory.h
    common.c common.h
    chunk.c chunk.h
    debug.h debug.c
    value.c value.h
    vm.h vm.c
    compiler.c compiler.h
    scanner.h scanner.c
)
