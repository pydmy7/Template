set(target demo)
add_executable(${target})
deploy(${target})

add_test(NAME ${target} COMMAND ${target})
