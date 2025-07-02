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

    struct Block {
        char blockMemory[BlockSize];
        Block *next = nullptr;
    };

    std::vector<Block*> m_blocks;  // All blocks
    std::vector<bool> m_usedBlocks;  // Used/free tracking
    std::unordered_map<void*, std::pair<size_t, size_t>> m_allocMap;  // ptr -> {startIndex, numBlocks}

    size_t _num_blocks_requested(size_t nBytes) {
        return (nBytes + BlockSize - 1) / BlockSize;  // ceiling division
    }

    size_t _find_allocated_block(void *ptr) {
        auto it = m_allocMap.find(ptr);
        if (it == m_allocMap.end()) {
            throw std::runtime_error("Pointer not found in allocation map");
        }
        return it->second.first;
    }

    size_t _find_number_of_allocated_blocks(void *ptr) {
        auto it = m_allocMap.find(ptr);
        if (it == m_allocMap.end()) {
            throw std::runtime_error("Pointer not found in allocation map");
        }
        return it->second.second;
    }

    void _release_block(Block *block) {
        for (size_t i = 0; i < m_blocks.size(); ++i) {
            if (m_blocks[i] == block) {
                m_usedBlocks[i] = false;
                return;
            }
        }
        throw std::runtime_error("Block not found");
    }

    void _allocate_block(Block *block) {
        for (size_t i = 0; i < m_blocks.size(); ++i) {
            if (m_blocks[i] == block) {
                if (m_usedBlocks[i]) {
                    throw std::runtime_error("Block already allocated");
                }
                m_usedBlocks[i] = true;
                return;
            }
        }
        throw std::runtime_error("Block not found");
    }

public:
    MemoryPool(size_t numberOfBytes) {
        m_numberOfBlocks = _num_blocks_requested(numberOfBytes);
        m_blocks.resize(m_numberOfBlocks);
        m_usedBlocks.resize(m_numberOfBlocks, false);

        for (size_t i = 0; i < m_numberOfBlocks; ++i) {
            m_blocks[i] = new Block();
        }
    }

    virtual ~MemoryPool() {
        for (auto block : m_blocks) {
            delete block;
        }
        m_blocks.clear();
        m_usedBlocks.clear();
        m_allocMap.clear();
    }

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
