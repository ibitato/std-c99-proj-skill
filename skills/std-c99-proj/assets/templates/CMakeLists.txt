cmake_minimum_required(VERSION 3.10)
project(c99_project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Strict compiler flags — zero warnings allowed
set(C99_FLAGS
    -std=c99 -pedantic
    -Wall -Wextra -Werror
    -Wconversion -Wsign-conversion -Wshadow -Wfloat-conversion
)

# Main executable
file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_options(${PROJECT_NAME} PRIVATE ${C99_FLAGS})

# Tests (opt-in)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    file(GLOB TEST_SOURCES "tests/*.c")
    # Library from src (excluding main.c) for linking tests
    file(GLOB LIB_SOURCES "src/*.c")
    list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.c$")
    add_library(project_lib STATIC ${LIB_SOURCES})
    target_include_directories(project_lib PUBLIC include)
    target_compile_options(project_lib PRIVATE ${C99_FLAGS})

    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_executable(${test_name} ${test_src})
        target_link_libraries(${test_name} PRIVATE project_lib)
        target_compile_options(${test_name} PRIVATE ${C99_FLAGS})
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
