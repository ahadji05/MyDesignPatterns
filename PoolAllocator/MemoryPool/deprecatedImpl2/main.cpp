#include "MemoryPoolContiguous.hpp"
#include <chrono>

constexpr size_t BLOCK_SIZE = 1024;
constexpr size_t POOL_SIZE = 1E+9;

int main() {

    ContiguousMemoryPool<BLOCK_SIZE> pool( POOL_SIZE );

    const auto start { std::chrono::steady_clock::now() };
    int i = 0;
    while ( i < 1000000 ) {
        char * ptr = ( char* ) pool.allocate( 1024 );
        if ( !ptr ) break;;

        int *a = ( int* ) ptr;
        int *b = ( int* ) ( ptr + 512 );
        for ( int j = 0; j < 128; ++j )
            a[j] += b[j];

        auto ptr1 = pool.allocate( 1024 );
        if ( !ptr1 ) break;

        auto ptr2 = pool.allocate( 1024 );
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

    pool.free_memory();

    return EXIT_SUCCESS;
}