#pragma once

#include "MemoryPool.hpp"

// Derived implementation
template<size_t BlockSize = 4096>
class ContiguousMemoryPool : public MemoryPool<BlockSize> {
    using Base = MemoryPool<BlockSize>;
    using Block = typename Base::Block;

public:
    ContiguousMemoryPool(size_t numberOfBytes)
        : Base(numberOfBytes) {}

    void *allocate(size_t nBytes) override {
        size_t blocksNeeded = this->_num_blocks_requested(nBytes);

        // Find contiguous free blocks
        size_t startIdx = this->m_numberOfBlocks;
        size_t count = 0;

        for (size_t i = 0; i < this->m_numberOfBlocks; ++i) {
            if (!this->m_usedBlocks[i]) {
                if (count == 0) startIdx = i;
                count++;
                if (count == blocksNeeded) break;
            } else {
                count = 0;
            }
        }

        if (count < blocksNeeded) {
            std::cerr << "Not enough contiguous blocks available\n";
            return nullptr;
        }

        // Mark blocks as used
        for (size_t i = 0; i < blocksNeeded; ++i) {
            this->_allocate_block(this->m_blocks[startIdx + i]);
        }

        // Record allocation metadata
        void* userPtr = static_cast<void*>(this->m_blocks[startIdx]->blockMemory);
        this->m_allocMap[userPtr] = { startIdx, blocksNeeded };

        return userPtr;
    }
};
