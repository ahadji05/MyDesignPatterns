
#include "AllocDeviceCuda.hpp"
#include "cuda_runtime.h"

template<typename T>
AllocDeviceCuda<T>::AllocDeviceCuda() noexcept {

    #ifdef DEBUG_ALLOCATORS
        std::cout << "AllocDeviceCuda Ctor\n";
    #endif

    this->m_kind = AllocatorKind::DEVICE_CUDA;
    this->m_name = "DEVICE_ALLOCATOR";
    this->m_numberOfAllocations  = 0;
    this->m_numberOfDeallocations = 0;
    this->m_totalAllocatedMemory = 0;
    this->m_totalDellocatedMemory = 0;
    this->m_currentMemoryUsage = 0;
}


template<typename T>
T * AllocDeviceCuda<T>::allocate( size_t nElements ) {

    if ( this->m_kind != AllocatorKind::DEVICE_CUDA )
        throw std::runtime_error("The allocator with name "+this->m_name+" is not compatible with AllocatorKind::DEVICE_CUDA!");

    #ifdef DEBUG_ALLOCATORS
        std::cout << "Allocating on device!\n";
    #endif

    ++this->m_numberOfAllocations;
    size_t nbytes = nElements * sizeof( T );
    this->m_totalAllocatedMemory += nbytes;
    this->m_currentMemoryUsage += nbytes;

    T * p;
    cudaError_t status = cudaMalloc ( (void**)&p, nbytes );
        if ( status != cudaSuccess )
        throw std::runtime_error("AllocDeviceCuda failed to allocate memory on device!");

    return p;
}



template<typename T>
void AllocDeviceCuda<T>::deallocate( T * p, size_t nElements ) {
    ++this->m_numberOfDeallocations;
    this->m_totalDellocatedMemory += ( nElements * sizeof( T ) );
    this->m_currentMemoryUsage -= ( nElements * sizeof( T ) );

    #ifdef DEBUG_ALLOCATORS
        std::cout << "Deallocating on device!\n";
    #endif

    cudaError_t status = cudaFree( p );
    if ( status != cudaSuccess )
        throw std::runtime_error("AllocDeviceCuda failed to deallocate memory on device!");
    p = nullptr;
}

#define X(TYPE) INSTANTIATE_CLASS_WITH_TYPE(AllocDeviceCuda, TYPE)
ALLOCATOR_TYPES_X
#undef X
