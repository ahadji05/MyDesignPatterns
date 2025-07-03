#pragma once

#include "AllocatorFactory.hpp"

#include <cstdlib>
#include <stdexcept>

/*************************************************************************************************************
 * \brief AllocHostMalloc is a template class that provides an allocator for host memory using malloc.
 * 
 * This class inherits from AllocatorFactory and implements the allocate and deallocate methods
 * to manage memory on the host using the standard C library's malloc and free functions.
 * 
 * \tparam T The type of elements to be allocated on the host.
 */
template<class T> 
class AllocHostMalloc : public AllocatorFactory<T> {
    public:
        using value_type = T;

        /*************************************************************************************************************
         * \brief Default destructor for AllocHostMalloc.
         */
        ~AllocHostMalloc() = default;
        
        /*************************************************************************************************************
         * \brief Constructs an AllocHostMalloc allocator.
         * \throws std::runtime_error if the allocator is not compatible with AllocatorKind::HOST_MALLOC.
         * \note This constructor initializes the allocator kind to AllocatorKind::HOST_MALLOC.
         * \note It is marked as noexcept to indicate that it does not throw exceptions.
         * \details The constructor sets the allocator kind, name, and initializes allocation statistics.
         *         It is designed to be used when the allocator is intended for host memory allocation using
         *         the standard C library's malloc and free functions.
         */
        AllocHostMalloc() noexcept ;

        /*************************************************************************************************************
         * \brief Allocates memory on the host using malloc.
         * \param nElements Number of elements to allocate.
         * \return Pointer to the allocated memory.
         * \throws std::runtime_error if the allocator is not compatible with AllocatorKind::
         */
        T * allocate( size_t nElements ) override;

        /*************************************************************************************************************
         * \brief Deallocates memory on the host using free.
         * \param p Pointer to the memory to be deallocated.
         * \param nElements Number of elements to deallocate.
         * \throws std::runtime_error if the allocator is not compatible with AllocatorKind::
         */
        void deallocate( T * p, size_t nElements ) override;
};
