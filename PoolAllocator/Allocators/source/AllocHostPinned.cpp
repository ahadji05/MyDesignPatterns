
#include "AllocHostPinned.hpp"
#include "cuda_runtime.h"

template<typename T>
AllocHostPinned<T>::AllocHostPinned() noexcept {

    #ifdef DEBUG_ALLOCATORS
    std::cout << "AllocHostPinned Ctor\n";
    #endif

    this->m_kind = AllocatorKind::HOST_PINNED_CUDA;
    this->m_name = "PINNED_ALLOCATOR";
    this->m_numberOfAllocations  = 0;
    this->m_numberOfDeallocations = 0;
    this->m_totalAllocatedMemory = 0;
    this->m_totalDellocatedMemory = 0;
    this->m_currentMemoryUsage = 0;
}

template<typename T>
T* AllocHostPinned<T>::allocate( size_t nElements ) {

    if ( this->m_kind != AllocatorKind::HOST_PINNED_CUDA )
        throw std::runtime_error("The allocator with name " + this->m_name + " is not compatible with AllocatorKind::HOST_PINNED_CUDA!");

    #ifdef DEBUG_ALLOCATORS
        std::cout << "Allocating pinned on host!\n";
    #endif

    ++this->m_numberOfAllocations;
    size_t nbytes = nElements * sizeof(T);
    this->m_totalAllocatedMemory += nbytes;
    this->m_currentMemoryUsage += nbytes;

    T *p = nullptr;
    cudaError_t status = cudaMallocHost( (void**)&p, nbytes );
    if ( status != cudaSuccess )
        throw std::runtime_error("AllocHostPinned failed to allocate pinned memory on host!");

    return p;
}


template<typename T>
void AllocHostPinned<T>::deallocate( T* p, size_t nElements ) {

    ++this->m_numberOfDeallocations;
    this->m_totalDellocatedMemory += ( nElements * sizeof(T) );
    this->m_currentMemoryUsage -= ( nElements * sizeof(T) );

    #ifdef DEBUG_ALLOCATORS
        std::cout << "Deallocating pinned on host!\n";
    #endif

    cudaError_t status = cudaFreeHost( p );
    if ( status != cudaSuccess )
        throw std::runtime_error("AllocHostPinned failed to dellocate pinned memory on host!");
    p = nullptr;
}

#define X(TYPE) INSTANTIATE_CLASS_WITH_TYPE( AllocHostPinned, TYPE )
ALLOCATOR_TYPES_X
#undef X