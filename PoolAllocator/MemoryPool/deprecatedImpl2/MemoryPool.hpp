#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cstddef>
#include <cstring>

template<size_t BlockSize = 4096>
class MemoryPool {
protected:
    size_t m_numberOfBlocks = 0;
    char * m_pool = nullptr;

    struct Block {
        char *blockMemory = nullptr;
    };

    std::vector<Block*> m_blocks;
    std::vector<bool> m_usedBlocks;
    std::unordered_map<void*, std::pair<size_t, size_t>> m_allocMap;
    std::unordered_map<Block*,int> m_blockIndex;
    size_t m_lastFreedOrAllocBlock = 0;

    size_t _num_blocks_requested( size_t nBytes );

    size_t _find_allocated_block( void *ptr );

    size_t _find_number_of_allocated_blocks(void *ptr );

    void _release_block( Block *block );

    void _allocate_block( Block *block );

public:
    MemoryPool( size_t numberOfBytes );

    void free_memory();

    virtual ~MemoryPool(){}

    virtual void *allocate( size_t nBytes ) = 0;

    virtual void deallocate( void *ptr ) = 0;
};
