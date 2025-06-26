
#include "MemoryPoolContiguous.hpp"

template<size_t BlockSize>
MemoryPool<BlockSize>::MemoryPool(size_t numberOfBytes) {
    m_numberOfBlocks = _num_blocks_requested(numberOfBytes);
    m_blocks.resize(m_numberOfBlocks);
    m_usedBlocks.resize(m_numberOfBlocks, false);
    m_pool = new char[m_numberOfBlocks*BlockSize];
    this->m_lastFreedOrAllocBlock = 0;
    std::cout << m_numberOfBlocks*BlockSize << std::endl;

    for (size_t i = 0; i < m_numberOfBlocks; ++i) {
        m_blocks[i] = new Block();
        m_blocks[i]->blockMemory = m_pool + ( i * BlockSize );
        m_blockIndex[m_blocks[i]] = i;
    }
}

template<size_t BlockSize>
size_t MemoryPool<BlockSize>::_num_blocks_requested(size_t nBytes) {
    return (nBytes + BlockSize - 1) / BlockSize;  // ceiling division
}

template<size_t BlockSize>
size_t MemoryPool<BlockSize>::_find_allocated_block(void *ptr) {
    auto it = m_allocMap.find(ptr);
    if (it == m_allocMap.end())
        throw std::runtime_error("Pointer not found in allocation map");
    return it->second.first;
}

template<size_t BlockSize>
size_t MemoryPool<BlockSize>::_find_number_of_allocated_blocks(void *ptr) {
    auto it = m_allocMap.find(ptr);
    if (it == m_allocMap.end())
        throw std::runtime_error("Pointer not found in allocation map");
    return it->second.second;
}

template<size_t BlockSize>
void MemoryPool<BlockSize>::_release_block(Block *block) {
    auto it = m_blockIndex.find(block);
    if ( it == m_blockIndex.end() )
        throw std::runtime_error("Block not found");
    
    if ( m_usedBlocks.at(it->second) == false )
        throw std::runtime_error("It is already false");

    m_usedBlocks[it->second] = false;
    this->m_lastFreedOrAllocBlock = it->second;
}

template<size_t BlockSize>
void MemoryPool<BlockSize>::_allocate_block(Block *block) {
    auto it = m_blockIndex.find(block);
    if ( it == m_blockIndex.end() )
        throw std::runtime_error("Block not found");

    if ( m_usedBlocks.at(it->second) == true )
        throw std::runtime_error("It is already true");

    m_usedBlocks[it->second] = true;
    this->m_lastFreedOrAllocBlock = it->second;
}

template<size_t BlockSize>
void MemoryPool<BlockSize>::free_memory() {
    for (size_t i = 0; i < m_numberOfBlocks; ++i)
        delete m_blocks[i];
    delete [] m_pool;
    m_blocks.clear();
    m_usedBlocks.clear();
    m_allocMap.clear();
    m_blockIndex.clear();
}

template<size_t BlockSize>
void * ContiguousMemoryPool<BlockSize>::allocate(size_t nBytes) {

    size_t blocksNeeded = this->_num_blocks_requested(nBytes);

    // Find contiguous free blocks
    size_t startIdx = this->m_numberOfBlocks;
    size_t count = 0;

    for (size_t i = this->m_lastFreedOrAllocBlock; i < this->m_numberOfBlocks; ++i) {
        if (!this->m_usedBlocks[i]) {
            if (count == 0) startIdx = i;
            count++;
            if (count == blocksNeeded) break;
        } else {
            count = 0;
        }
    }

    // if did not manage to find in the range: [m_lastFreedOrAllocBlock - end]
    // try search in the range: [begin - m_lastFreedOrAllocBlock]
    if ( count < blocksNeeded ) {
        for (size_t i = 0; i < this->m_lastFreedOrAllocBlock + blocksNeeded; ++i) {
            if (!this->m_usedBlocks[i]) {
                if (count == 0) startIdx = i;
                count++;
                if (count == blocksNeeded) break;
            } else {
                count = 0;
            }
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

template class MemoryPool<128>;
template class ContiguousMemoryPool<128>;