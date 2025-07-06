#pragma once

#include "MemoryPool.hpp"

/*************************************************************************************************************
 * @brief Implements a memory pool for CUDA memory management.
 * This class allocates memory using cudaMalloc and deallocates it using cudaFree.
 */
class CudaMemoryPool : public MemoryPool {
    private:
        char * m_pool = nullptr;

    public:
        CudaMemoryPool( size_t numberOfBytes, size_t blockSize );
        ~CudaMemoryPool();
        void *allocate( size_t nBytes );
        void  deallocate( void *p );
};
