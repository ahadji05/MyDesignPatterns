
#include "CudaMemoryPool.hpp"

#include "cuda_runtime.h"
#include "cuda_runtime_api.h"

void *CudaMemoryPool::allocate( size_t nBytes )
{
    return this->do_allocate( nBytes );
}

void CudaMemoryPool::deallocate( void * p )
{
    return this->do_deallocate( p );
}

CudaMemoryPool::CudaMemoryPool( size_t numberOfBytes, size_t blockSize )
{
    // first, we call the base-class constructor to initilize internal member variables
    this->initialize_memory_pool( numberOfBytes, blockSize );

    // then, we allocate the memory of the pool
    cudaError_t status = cudaMalloc( &m_pool, this->m_numberOfBlocks * this->m_blockSize);
    if ( status != cudaSuccess )
        throw std::runtime_error("CudaMemoryPool failed to allocate pool!");

    // finally, we set the blockMemory pointers at fixed intervals based on the block size.
    for ( size_t i = 0; i < this->m_numberOfBlocks; ++i )
        this->m_blocks[i]->blockMemory = m_pool + ( i * this->m_blockSize );
}

CudaMemoryPool::~CudaMemoryPool()
{
    cudaError_t status = cudaFree(m_pool);
    if ( status != cudaSuccess )
        std::cout << "CudaMemoryPool failed to free pool!" << std::endl;
}
