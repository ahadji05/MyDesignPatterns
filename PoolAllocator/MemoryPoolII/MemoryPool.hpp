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
    size_t m_numberOfBlocks;
    char * m_pool = nullptr;

    struct Block {
        char *blockMemory;
    };

    std::vector<Block*> m_blocks;
    std::vector<bool> m_usedBlocks;
    std::unordered_map<void*, std::pair<size_t, size_t>> m_allocMap;  // ptr -> {startIndex, numBlocks}
    std::unordered_map<Block*,int> m_blockIndex;
    int m_lastFreedOrAllocBlock;

    size_t _num_blocks_requested(size_t nBytes);

    size_t _find_allocated_block(void *ptr);

    size_t _find_number_of_allocated_blocks(void *ptr);

    void _release_block(Block *block);

    void _allocate_block(Block *block);

public:
    MemoryPool(size_t numberOfBytes);

    void free_memory();

    virtual ~MemoryPool(){}

    virtual void *allocate(size_t nBytes) = 0;

    virtual void deallocate(void *ptr) {
        try {
            size_t startIndex = _find_allocated_block(ptr);
            size_t numBlocks = _find_number_of_allocated_blocks(ptr);
            for (size_t i = 0; i < numBlocks; ++i) {
                _release_block(m_blocks[startIndex + i]);
            }
            m_allocMap.erase(ptr);
        } catch (...) {
            std::cerr << "Attempted to deallocate invalid pointer" << std::endl;
        }
    }
};
