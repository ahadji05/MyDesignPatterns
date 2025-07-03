#pragma once

#include "AllocatorFactory.hpp"
#include <cstdlib>
#include <stdexcept>

/*************************************************************************************************************
 * \brief AllocDeviceCuda is a template class that provides an allocator for device memory using CUDA.
 * 
 * This class inherits from AllocatorFactory and implements the allocate and deallocate methods
 * to manage memory on the CUDA device. It is designed to be used with types that can be allocated
 * on the device, such as CUDA-compatible data types.
 * 
 * \tparam T The type of elements to be allocated on the device.
 */
template<class T> 
class AllocDeviceCuda : public AllocatorFactory<T> {
    public:
        using value_type = T;

        /*************************************************************************************************************
         * \brief Constructs an AllocDeviceCuda allocator.
         * \throws std::runtime_error if the allocator is not compatible with AllocatorKind::DEVICE_CUDA.
         * \note This constructor initializes the allocator kind to AllocatorKind::DEVICE_CUDA.
         * \note It is marked as noexcept to indicate that it does not throw exceptions.
         */
        AllocDeviceCuda() noexcept ;

        /*************************************************************************************************************
         * \brief Default destructor for AllocDeviceCuda.
         */
        ~AllocDeviceCuda() = default;

        /*************************************************************************************************************
         * \brief Allocates memory on the device using cudaMalloc.
         * \param nElements Number of elements to allocate.
         * \return Pointer to the allocated memory.
         * \throws std::runtime_error if the allocator is not compatible with AllocatorKind::DEVICE_CUDA.
         */
        T * allocate( size_t nElements ) override ;
        
        /*************************************************************************************************************
         * \brief Deallocates memory on the device using cudaFree.
         * \param p Pointer to the memory to be deallocated.
         * \param nElements Number of elements to deallocate.
         */
        void deallocate( T * p, size_t nElements ) override;
};
