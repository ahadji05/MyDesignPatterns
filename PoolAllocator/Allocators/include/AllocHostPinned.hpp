#pragma once

#include "AllocatorFactory.hpp"

#include <cstdlib>
#include <stdexcept>

/*************************************************************************************************************
 * \brief AllocHostPinned is a template class that provides an allocator for pinned host memory using CUDA.
 * 
 * This class inherits from AllocatorFactory and implements the allocate and deallocate methods
 * to manage memory on the pinned host, which is useful for high-performance data transfers between host 
 * and device.
 * 
 * \tparam T The type of elements to be allocated on the pinned host.
 */
template<class T>
class AllocHostPinned : public AllocatorFactory<T> {
    public:
        using value_type = T;

        /*************************************************************************************************************
         * \brief Constructs an AllocHostPinned allocator.
         * \throws std::runtime_error if the allocator is not compatible with AllocatorKind::HOST_PINNED.
         * \note This constructor initializes the allocator kind to AllocatorKind::HOST_PINNED.
         * \note It is marked as noexcept to indicate that it does not throw exceptions.
         */
        AllocHostPinned() noexcept;

        /*************************************************************************************************************
         * \brief Default destructor for AllocHostPinned.
         */
        ~AllocHostPinned() = default;

        /*************************************************************************************************************
         * \brief Allocates memory on the host using cudaMallocHost.
         * \param nElements Number of elements to allocate.
         * \return Pointer to the allocated memory.
         * \throws std::runtime_error if the allocator is not compatible with AllocatorKind::HOST_PINNED.
         */
        T * allocate( size_t nElements ) override;

        /*************************************************************************************************************
         * \brief Deallocates memory on the host using cudaFreeHost.
         * \param p Pointer to the memory to be deallocated.
         * \param nElements Number of elements to deallocate.
         */
        void deallocate( T * p, size_t nElements ) override;
};
