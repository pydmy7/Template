cmake_minimum_required(VERSION 3.21)

project(RenderEngine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(TEMPLATE_BUILD_TESTS "Build tests" OFF)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "[INFO] Global use of ccache to accelerate compile")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

include(external/FindLibray.cmake)
add_subdirectory(external)

add_subdirectory(cmake)

add_subdirectory(src)

if (TEMPLATE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
