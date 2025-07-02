#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cstddef>
#include <cstring>

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

        void  initialize_memory_pool( size_t numberOfBytes, size_t blockSize );
        void *do_allocate( size_t nBytes );
        void  do_deallocate( void *ptr );
        MemoryPool();
        ~MemoryPool();
};
