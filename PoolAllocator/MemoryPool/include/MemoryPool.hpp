#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cstddef>
#include <cstring>


/*************************************************************************************************************
 * @brief MemoryPool is a class that manages a pool of memory blocks for controlled memory allocations and 
 * deallocations. When the pool is initialized, it allocates a specified number of memory blocks of a given 
 * size and then every time a memory allocation is requested, it checks if there are any free blocks available 
 * and provides one, or more, depending on the request size. It is not necessrily faster than malloc/free, it 
 * is comparable in speed, but it allows to build on top of it applications that require a controlled memory 
 * allocation strategy, that will not fail during runtime due to insufficient memory.
 * 
 * @note This class is not thread-safe. If you need to use it in a multithreaded environment, you should 
 * implement your own synchronization mechanism to ensure that only one thread accesses the memory pool 
 * at a time.
 * 
 * @note MemoryPool is deigned to be used a base class for other memory pool implementations. It creates the 
 * blocks but does not allocate the actual memory of the pool. Instead, this is done in the derived classes: 
 * e.g. `HostMemoryPool`, which allocates memory using new/delete, or `CudaMemoryPool`, which allocate memory 
 * using cudaMalloc/cudaFree.
 */
class MemoryPool {
    private:
        struct Block {
            char *blockMemory = nullptr;
        };
        std::unordered_map<void*, std::pair<size_t, size_t>> m_allocMap;
        std::unordered_map<Block*,int>                       m_blockIndex;
        std::vector<bool>                                    m_usedBlocks;
        size_t                                               m_lastFreedOrAllocBlock = 0;

        std::pair<size_t,size_t> _find_allocated_block( void *ptr );
        size_t                   _num_blocks_requested( size_t nBytes );
        void                     _release_block( Block *block );
        void                     _allocate_block( Block *block );

    protected:
        std::vector<Block*> m_blocks;
        size_t              m_numberOfBlocks = 0;
        size_t              m_blockSize      = 0;


        /*************************************************************************************************************
         * @brief initializes the internal member variables of the MemoryPool class.
         * It creates the blocks and registers them in the m_blockIndex map, which allows
         * to identify each block by its index.
         * 
         * @param numberOfBytes The total size of memory to allocate for the pool.
         * @param blockSize The size of each individual block in the pool.
         */
        void  initialize_memory_pool( size_t numberOfBytes, size_t blockSize );


        /*************************************************************************************************************
         * @brief do_allocate is a protected method that allocates memory from the pool.
         * It checks if there are any free blocks available and returns a pointer to the allocated memory.
         * If not enough consecutive blocks are available, it returns nullptr.
         * 
         * @param nBytes The number of bytes to allocate.
         * @return A pointer to the allocated memory, or nullptr if not enough blocks are available
         */
        void *do_allocate( size_t nBytes );


        /*************************************************************************************************************
         * @brief do_deallocate is a protected method that deallocates memory from the pool.
         * It marks the blocks as free and updates the allocation map.
         * 
         * @param ptr A pointer to the memory to deallocate.
         * @note The pointer must have been allocated from this memory pool.
         */
        void  do_deallocate( void *ptr );

        MemoryPool();
        ~MemoryPool();
};
