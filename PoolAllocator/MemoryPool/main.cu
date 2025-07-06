
#include "Dispatching.hpp"
#include "SaxpyKernel.hpp"

#include <chrono>
#include <vector>

const size_t POOL_SIZE = 1E+9;

int main() {
try
{
    HostMemoryPool pool( POOL_SIZE, 4096 );

    const auto start { std::chrono::steady_clock::now() };
    int k = 0;
    while ( k < 1 ) {
        char * ptr = ( char* ) pool.allocate( 100000 );
        if ( !ptr ) break;

        float a = 1.5;
        float *x = ( float* ) ptr;
        float *y = ( float* ) ( ptr + 50000 );

        // zero-copy dispatch of SaxpyKernel based on the type of the pool
        dispatch_from( pool, SaxpyKernel{}, 12500, a, x, y );

        auto ptr1 = pool.allocate( 4096 );
        if ( !ptr1 ) break;

        auto ptr2 = pool.allocate( 4097 );
        if ( !ptr2 ) break;

        pool.deallocate( ptr2 );
        pool.deallocate( ptr1 );
        pool.deallocate( ptr );

        ++k;
        if ( k % 100000 == 0 ) std::cout << k << std::endl;
    }
    std::cout << "max k : " <<  k << std::endl;
    const auto finish { std::chrono::steady_clock::now() };
    const std::chrono::duration<double> elapsed_seconds{ finish - start };
    std::cout << "time: " << elapsed_seconds.count() << " seconds\n";
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}
    return EXIT_SUCCESS;
}
