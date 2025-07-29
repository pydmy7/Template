set(target demo)
add_executable(${target})
deploy(${target})

target_link_libraries(${target} PRIVATE
    StandardCodeLibrary
)

add_test(NAME ${target} COMMAND ${target})
