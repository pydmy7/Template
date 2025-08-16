cmake_minimum_required(VERSION 3.21)

project(Template)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(TEMPLATE_BUILD_TESTS "Build tests" OFF)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "[INFO] Global use of ccache to accelerate compile")
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    set(CMAKE_CUDA_COMPILER_LAUNCHER ccache)
endif(CCACHE_FOUND)

include(external/FindLibray.cmake)
add_subdirectory(external)

add_subdirectory(cmake)

add_subdirectory(src)

if (TEMPLATE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
