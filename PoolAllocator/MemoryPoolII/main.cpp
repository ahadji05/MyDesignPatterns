
#include "AlignedMemoryPool.hpp"
#include <chrono>

const size_t POOL_SIZE = 1E+9;

int main() {

    AlignedMemoryPool pool( POOL_SIZE );

    const auto start { std::chrono::steady_clock::now() };
    int i = 0;
    while ( i < 1E6 ) {
        char * ptr = ( char* ) pool.allocate( 4000 );
        if ( !ptr ) break;

        int *a = ( int* ) ptr;
        int *b = ( int* ) ( ptr + 512 );
        for ( int j = 0; j < 128; ++j )
            a[j] += b[j];

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
