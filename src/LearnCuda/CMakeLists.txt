enable_language(CUDA)

set(CUDA_SEPARABLE_COMPILATION ON)
set(CUDA_STANDARD 17)
set(CUDA_STANDARD_REQUIRED ON)

set(target LearnCuda)
add_executable(${target} src/main.cu)

find_package(CUDAToolkit REQUIRED)

target_link_libraries(${target} PRIVATE CUDA::cudart)
