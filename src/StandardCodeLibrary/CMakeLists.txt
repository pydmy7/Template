set(target StandardCodeLibrary)

add_library(${target} OBJECT)
target_sources(${target} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cache.cpp)
target_include_directories(${target} PUBLIC include)
