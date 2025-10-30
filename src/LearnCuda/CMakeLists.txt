set(target LearnCuda)
add_executable(${target} src/main.cu)

target_compile_options(${target} PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:--extended-lambda>
    $<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>
)

generatePath(${target} "${PROJECT_SOURCE_DIR}/target_path.toml")
