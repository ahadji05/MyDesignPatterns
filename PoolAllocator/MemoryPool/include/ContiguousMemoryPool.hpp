#pragma once

#include "MemoryPool.hpp"

class ContiguousMemoryPool : public MemoryPool {
    private:
        char * m_pool = nullptr;

    public:
        ContiguousMemoryPool( size_t numberOfBytes, size_t blockSize );
        ~ContiguousMemoryPool();
        void *allocate( size_t nBytes );
        void  deallocate( void *p );
};
