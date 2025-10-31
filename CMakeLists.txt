cmake_minimum_required(VERSION 3.24)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(TEMPLATE_BUILD_TESTS "Build tests" ON)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "[INFO] Global use of ccache to accelerate compile")
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    set(CMAKE_CUDA_COMPILER_LAUNCHER ccache)
endif(CCACHE_FOUND)

project(Template LANGUAGES C CXX CUDA)

include(external/FindLibray.cmake)
add_subdirectory(external)

add_subdirectory(cmake)

add_subdirectory(src)

if (TEMPLATE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
