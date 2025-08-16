enable_language(CUDA)

set(target LearnCuda)
add_executable(${target} src/main.cu)

find_package(CUDAToolkit REQUIRED)

set_target_properties(${target} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_compile_options(${target} PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:--extended-lambda>
    $<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>
)
target_compile_features(${target} PRIVATE cxx_std_17 cuda_std_17)

generatePath(${target} "${PROJECT_SOURCE_DIR}/target_path.toml")
