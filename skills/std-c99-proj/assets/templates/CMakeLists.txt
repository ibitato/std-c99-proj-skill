cmake_minimum_required(VERSION 3.10)
project(c99_project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- Warning flags (always active) ---
add_compile_options(
    -Wall -Wextra -Werror
    -Wconversion -Wsign-conversion -Wshadow -Wfloat-conversion
    -pedantic
    -fstack-protector-strong
)

# --- Debug: no optimization, full debug info ---
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")

# --- Release: optimized + hardened ---
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -D_FORTIFY_SOURCE=2 -fPIE")

# GCC >= 10: enable -fanalyzer (compile-time static analysis)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    if(NOT CMAKE_C_COMPILER_VERSION VERSION_LESS "10")
        string(APPEND CMAKE_C_FLAGS_DEBUG " -fanalyzer")
    endif()
endif()

# Main executable
file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Tests (opt-in)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    file(GLOB TEST_SOURCES "tests/*.c")
    file(GLOB LIB_SOURCES "src/*.c")
    list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.c$")
    add_library(project_lib STATIC ${LIB_SOURCES})
    target_include_directories(project_lib PUBLIC include)

    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_executable(${test_name} ${test_src})
        target_link_libraries(${test_name} PRIVATE project_lib)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
