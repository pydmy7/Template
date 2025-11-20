set(target libavoid)
add_library(${target} STATIC)
deploy(${target})
target_compile_definitions(${target} PRIVATE LIBAVOID_NO_DLL)
