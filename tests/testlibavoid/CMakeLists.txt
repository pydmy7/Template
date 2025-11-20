set(target testlibavoid)
add_executable(${target}
    example.cpp
)

target_link_libraries(${target} PRIVATE libavoid)

copyDll(${target})
generatePath(${target} "${PROJECT_SOURCE_DIR}/target_path.toml")
