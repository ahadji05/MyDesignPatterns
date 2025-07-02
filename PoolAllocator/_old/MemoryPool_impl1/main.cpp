


#include "MemoryPoolContiguous.hpp"

/**
 * 
 */
void memory_pool_example(){
    constexpr size_t BLOCK_SIZE = 128;
    constexpr size_t POOL_SIZE = 1E+7;

    ContiguousMemoryPool<BLOCK_SIZE> pool(POOL_SIZE);

    int i=0;
    while (i < 80000) {
        void* ptr = pool.allocate(128);
        // void* ptr = malloc(128);
        if ( !ptr ) return;
        //     std::cout << "Allocated 128 bytes at: " << ptr << std::endl;
        // else 
        //     return;

        auto ptr1 = pool.allocate(256);  // Allocates 2 blocks (say, blocks 0 and 1)
        // auto ptr1 = malloc(256);  // Allocates 2 blocks (say, blocks 0 and 1)
        // if (ptr1)
        //     std::cout << "Allocated 256 bytes at: " << ptr1 << std::endl;
        pool.deallocate(ptr1);           // Blocks 0 and 1 marked as free
        // free(ptr1);           // Blocks 0 and 1 marked as free

        auto ptr2 = pool.allocate(256);  // Should reuse blocks 0 and 1 again
        // auto ptr2 = malloc(256);  // Should reuse blocks 0 and 1 again
        // if (ptr2)
        //     std::cout << "Allocated 256 bytes at: " << ptr2 << std::endl;
        pool.deallocate(ptr2);
        // free(ptr2);

        // std::cout << "i: " << i << std::endl;
        ++i;
        if ( i % 1000 == 0 )std::cout << i << std::endl;
    }

    std::cout << "max i : " <<  i << std::endl;
}

int main( int argc, char *argv[] ) {

    if ( argc )
        std::cout << "\nRunning program: " << argv[0] << std::endl << std::endl;

    try {
        memory_pool_example();
    } catch( const std::exception & e ) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
