
#include "Dispatching.hpp"

void saxpy_cpu( int n, float a, float const *__restrict__ const x, float *__restrict__ const y );

__global__ void saxpy_gpu( int n, float a, float const *__restrict__ const x, float *__restrict__ const y );

/*************************************************************************************************************
 * Functor for saxpy-kernel dispatching
 */
struct SaxpyKernel {

    template<typename Tag>
    void operator()( int n, float a, float const * const x, float * const y ) const {
        if constexpr ( is_cpu_pool_v<Tag> ) {
            std::cout << "CPU SAXPY...\n";
            saxpy_cpu(n, a, x, y);
        } else if constexpr ( is_gpu_pool_v<Tag> ) {
            std::cout << "CUDA SAXPY...\n";
            saxpy_gpu<<<1 + n/1024, 1024>>>(n, a, x, y);
        } else
            THROW_TAG_DISPATCH_ERROR(SaxpyKernel,Tag)
    }
};
