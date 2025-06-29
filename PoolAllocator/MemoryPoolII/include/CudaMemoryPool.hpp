#pragma once

#include "MemoryPool.hpp"

class CudaMemoryPool : public MemoryPool {
    private:
        char * m_pool = nullptr;

    public:
        CudaMemoryPool( size_t numberOfBytes, size_t blockSize );
        ~CudaMemoryPool();
        void *allocate( size_t nBytes );
        void  deallocate( void *p );
};
