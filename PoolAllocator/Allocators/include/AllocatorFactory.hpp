#pragma once

#include <iostream>
#include <cstddef>
#include <memory>

enum class AllocatorKind {
    HOST_MALLOC,
    HOST_PINNED_CUDA,
    DEVICE_CUDA
};

/*************************************************************************************************************
 * \brief AllocatorFactory is a base class for all allocators.
 * 
 * This class provides a common interface for allocators, including methods for allocation and deallocation,
 * as well as statistics about the allocator's usage.
 * 
 * \tparam T The type of elements to be allocated by the allocator.
 */
template<typename T>
class AllocatorFactory {
    protected:
        AllocatorKind m_kind;
        std::string   m_name;
        size_t        m_numberOfAllocations, m_numberOfDeallocations;
        size_t        m_totalAllocatedMemory, m_totalDellocatedMemory;
        size_t        m_currentMemoryUsage;

    public:
        using value_type = T;

        /*************************************************************************************************************
         * \brief Returns the kind of allocator.
         */
        AllocatorKind getKind()                   const { return this->m_kind;                  }

        /*************************************************************************************************************
         * * \brief Returns the name of the allocator.
         */
        std::string   getName()                   const { return this->m_name;                  }

        /*************************************************************************************************************
         * \brief Returns the statistics of the allocator.
         * 
         * These statistics include the number of allocations, deallocations,
         * total allocated memory, total deallocated memory, and current memory usage.
         */
        size_t        getNumberOfAllocations()    const { return this->m_numberOfAllocations;   }

        /*************************************************************************************************************
         * * \brief Returns the number of deallocations made by the allocator.
         */
        size_t        getNumberOfDeallocations()  const { return this->m_numberOfDeallocations; }

        /*************************************************************************************************************
         * \brief Returns the total allocated memory, total deallocated memory, and current memory usage.
         */
        size_t        getTotalAllocatedMemory()   const { return this->m_totalAllocatedMemory;  }

        /*************************************************************************************************************
         * \brief Returns the total deallocated memory.
         */
        size_t        getTotalDeallocatedMemory() const { return this->m_totalDellocatedMemory; }

        /*************************************************************************************************************
         * \brief Returns the current memory usage of the allocator.
         */
        size_t        getCurrentMemoryUsage()     const { return this->m_currentMemoryUsage;    }
        
        /*************************************************************************************************************
         * \brief Default constructor for AllocatorFactory.
         * 
         * Initializes the allocator kind, name, and statistics.
         */
        AllocatorFactory()  noexcept { 
            #ifdef DEBUG_ALLOCATORS
            std::cout<<"Ctor\n";
            #endif 
        }

        /*************************************************************************************************************
         * \brief Default destructor for AllocatorFactory.
         */
        ~AllocatorFactory() noexcept {
            #ifdef DEBUG_ALLOCATORS
            std::cout<<"Dtor\n";
            #endif
        }

        /*************************************************************************************************************
         * \brief Pure virtual function to allocate memory for nElements of type T.
         */
        virtual T *allocate( size_t nElements ) = 0;


        /*************************************************************************************************************
         * \brief Pure virtual function to deallocate memory for nElements of type T.
         */
        virtual void deallocate( T *p, size_t nElements ) = 0;
};


/**
 * This is an X macro that is used to define the allocator types that will be instantiated.
 * It is used to avoid code duplication and to make it easier to add new allocator types in the future.
 * 
 * Only the `char` is needed for the AllocHostMalloc, AllocHostPinned, and AllocDeviceCuda allocators.
 * This is because the runtime manager will instantiate these allocators with the char type and other the 
 * AllocatorWrapper will handle the conversions to any other type, e.g., int, float, etc.
 */
#define ALLOCATOR_TYPES_X \
    X(char)

#define INSTANTIATE_CLASS_WITH_TYPE(CLASS, TYPE) \
    template class CLASS<TYPE>; 
