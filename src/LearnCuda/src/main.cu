#include <cstdio>

#include <cuda_runtime.h>

// CUDA kernel function
__global__ void helloFromGPU() {
    [[maybe_unused]] int xxx = threadIdx.x;
}

int main() {
    printf("Hello from CPU!\n");

    // Launch kernel with 4 threads
    helloFromGPU<<<1, 4>>>();

    // Wait for GPU to finish
    cudaDeviceSynchronize();

    return 0;
}
