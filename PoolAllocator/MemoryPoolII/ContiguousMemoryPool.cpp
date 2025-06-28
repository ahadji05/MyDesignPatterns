
#include "ContiguousMemoryPool.hpp"

ContiguousMemoryPool::ContiguousMemoryPool( size_t numberOfBytes, size_t blockSize )
{
    // first we call the base-class constructor to initilize internal member variables.
    this->initialize_memory_pool( numberOfBytes, blockSize );

    // then we allocate the memory of the pool and set the blockMemory pointers based 
    // on the implementation of our choice.
    m_pool = new char[this->m_numberOfBlocks * this->m_blockSize];

    for ( size_t i = 0; i < this->m_numberOfBlocks; ++i )
        this->m_blocks[i]->blockMemory = m_pool + ( i * this->m_blockSize );
}

ContiguousMemoryPool::~ContiguousMemoryPool()
{
    delete [] m_pool;
}