enable_language(CUDA)

set(target LearnCuda)
add_executable(${target} src/main.cu)

find_package(CUDAToolkit REQUIRED)

target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:--extended-lambda>)
target_compile_features(${target} PRIVATE cxx_std_17 cuda_std_17)
set_target_properties(${target} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
