#pragma once

#include "MemoryPool.hpp"

class HostMemoryPool : public MemoryPool {
    private:
        char * m_pool = nullptr;

    public:
        HostMemoryPool( size_t numberOfBytes, size_t blockSize );
        ~HostMemoryPool();
        void *allocate( size_t nBytes );
        void  deallocate( void *p );
};
