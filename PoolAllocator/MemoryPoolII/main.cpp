
#include "MemoryPoolContiguous.hpp"

int main() {

    constexpr size_t BLOCK_SIZE = 128;
    constexpr size_t POOL_SIZE = 1E+8;

    ContiguousMemoryPool<BLOCK_SIZE> pool(POOL_SIZE);

    int i=0;
    while (i < 100000) {
        char* ptr = (char*) pool.allocate(128);
        // char* ptr = (char*) malloc(128);
        if ( !ptr ) break;;

        int *a = (int*) ptr;
        int *b = (int*) (ptr + 64);
        for ( int j = 0; j < 16; ++j )
            a[j] += b[j];

        auto ptr1 = pool.allocate(256);
        // auto ptr1 = malloc(256);
        if ( !ptr1 ) break;


        auto ptr2 = pool.allocate(256);
        // auto ptr2 = malloc(256);
        if ( !ptr2 ) break;

        // pool.deallocate(ptr);
        // pool.deallocate(ptr1);
        // pool.deallocate(ptr2);

        // free(ptr);
        // free(ptr1);
        // free(ptr2);

        ++i;
        if ( i % 1000 == 0 )std::cout << i << std::endl;
    }

    std::cout << "max i : " <<  i << std::endl;

    pool.free_memory();

    return EXIT_SUCCESS;
}