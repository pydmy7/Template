set(target LearnCuda)
add_executable(${target} src/main.cu)

target_compile_options(${target} PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:--extended-lambda>
    $<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>
)

target_link_libraries(${target} PRIVATE Eigen3::Eigen)
target_compile_definitions(${target} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:EIGEN_DONT_VECTORIZE>)

generatePath(${target} "${PROJECT_SOURCE_DIR}/target_path.toml")
