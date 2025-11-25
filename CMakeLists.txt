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

if (NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
        set(CMAKE_CUDA_ARCHITECTURES native)
    else()
        message(STATUS "Running __nvcc_device_query for CMAKE_CUDA_ARCHITECTURES")
        execute_process(COMMAND "__nvcc_device_query"
            RESULT_VARIABLE NVCC_DEVICE_QUERY_RESULT
            OUTPUT_VARIABLE CMAKE_CUDA_ARCHITECTURES
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (NVCC_DEVICE_QUERY_RESULT EQUAL 0)
            message(STATUS "CMAKE_CUDA_ARCHITECTURES set to ${CMAKE_CUDA_ARCHITECTURES}")
        else()
            message(WARNING "CMAKE_CUDA_ARCHITECTURES not defined and __nvcc_device_query failed.")
            unset(CMAKE_CUDA_ARCHITECTURES)
        endif()
    endif()
endif()

project(Template LANGUAGES C CXX CUDA)

if (CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/$<CONFIG>)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
endif()

include(external/FindLibray.cmake)
add_subdirectory(external)

add_subdirectory(cmake)

add_subdirectory(src)

if (TEMPLATE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
