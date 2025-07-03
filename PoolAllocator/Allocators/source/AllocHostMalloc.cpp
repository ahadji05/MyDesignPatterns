
#include "AllocHostMalloc.hpp"


template<typename T>
AllocHostMalloc<T>::AllocHostMalloc() noexcept {

    #ifdef DEBUG_ALLOCATORS
        std::cout << "AllocHostMalloc Ctor\n";
    #endif

    this->m_kind = AllocatorKind::HOST_MALLOC;
    this->m_name = "HOST_ALLOCATOR";
    this->m_numberOfAllocations = 0;
    this->m_numberOfDeallocations = 0;
    this->m_totalAllocatedMemory = 0;
    this->m_totalDellocatedMemory = 0;
    this->m_currentMemoryUsage = 0;
}


template<typename T>
T* AllocHostMalloc<T>::allocate( size_t nElements ) {

    if ( this->m_kind != AllocatorKind::HOST_MALLOC )
        throw std::runtime_error("The allocator with name " + this->m_name + " is not compatible with AllocatorKind::HOST_MALLOC!");

    #ifdef DEBUG_ALLOCATORS
        std::cout << "Allocating on host! ( " << nElements * sizeof(T) << " ) bytes \n";
    #endif

    ++this->m_numberOfAllocations;
    size_t nbytes = nElements * sizeof(T);
    this->m_totalAllocatedMemory += nbytes;
    this->m_currentMemoryUsage += nbytes;

    T *p = nullptr;
    p = ( T* ) malloc( nbytes );
    if ( !p )
        throw std::runtime_error("AllocHostMalloc failed to allocate pageable memory on host!");

    return p;
}


template<typename T>
void AllocHostMalloc<T>::deallocate(T* p, size_t nElements) {

    ++this->m_numberOfDeallocations;
    this->m_totalDellocatedMemory += (nElements * sizeof(T));
    this->m_currentMemoryUsage -= (nElements * sizeof(T));              

    #ifdef DEBUG_ALLOCATORS
        std::cout << "Deallocating on host! ( " << nElements * sizeof(T) << " ) bytes \n";
    #endif

    free( p );
    p = nullptr;
}

#define X(TYPE) INSTANTIATE_CLASS_WITH_TYPE(AllocHostMalloc, TYPE)
ALLOCATOR_TYPES_X
#undef X
