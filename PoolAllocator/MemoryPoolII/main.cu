
#include "CudaMemoryPool.hpp"
#include "ContiguousMemoryPool.hpp"
#include "AlignedMemoryPool.hpp"
#include <chrono>

const size_t POOL_SIZE = 1E+9;

__global__ void saxpy(int n, float a, float *x, float *y) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        y[i] = a * x[i] + y[i];
    }
}

int main() {

    CudaMemoryPool pool( POOL_SIZE, 4096 );

    const auto start { std::chrono::steady_clock::now() };
    int i = 0;
    while ( i < 5 ) {
        char * ptr = ( char* ) pool.allocate( 100000 );
        if ( !ptr ) break;

        float *a = ( float* ) ptr;
        float *b = ( float* ) ( ptr + 50000 );
        // for ( int j = 0; j < 12500; ++j )
        //     a[j] += b[j];

        saxpy<<<99,1024>>>(100000, 1, a, b);

        auto ptr1 = pool.allocate( 4096 );
        if ( !ptr1 ) break;

        auto ptr2 = pool.allocate( 4097 );
        if ( !ptr2 ) break;

        pool.deallocate( ptr2 );
        pool.deallocate( ptr1 );
        pool.deallocate( ptr );

        ++i;
        if ( i % 100000 == 0 ) std::cout << i << std::endl;
    }
    std::cout << "max i : " <<  i << std::endl;
    const auto finish { std::chrono::steady_clock::now() };
    const std::chrono::duration<double> elapsed_seconds{ finish - start };
    std::cout << "time: " << elapsed_seconds.count() << " seconds\n";

    return EXIT_SUCCESS;
}
