set(target StandardCodeLibrary)
add_library(${target} INTERFACE)

file(GLOB_RECURSE headers
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
target_sources(${target} INTERFACE ${headers})
target_include_directories(${target} INTERFACE include)
