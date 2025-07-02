#pragma once

#include "MemoryPool.hpp"

template<size_t BlockSize = 4096>
class ContiguousMemoryPool : public MemoryPool<BlockSize> {

    using Base = MemoryPool<BlockSize>;
    using Block = typename Base::Block;

public:
    ContiguousMemoryPool(size_t numberOfBytes) : Base(numberOfBytes) {}

    void *allocate( size_t nBytes );

    void deallocate( void * p );
};