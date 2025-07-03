#pragma once

#include "AllocatorFactory.hpp"

#include <memory>

/*************************************************************************************************************
 * \brief AllocatorWrapper is a template class that wraps an AllocatorFactory to provide a standard allocator 
 * interface.
 * 
 * This class allows for the use of custom allocators with standard containers by providing
 * an interface compatible with the C++ Standard Library's allocator requirements.
 * 
 * \tparam T The type of elements to be allocated.
 */
template <typename T>
class AllocatorWrapper {
    private:
        std::shared_ptr<AllocatorFactory<char>> m_wrapper;

    public:
        using value_type = T;

        /*************************************************************************************************************
         * \brief Constructs an AllocatorWrapper with a shared pointer to an AllocatorFactory.
         */
        AllocatorWrapper( std::shared_ptr<AllocatorFactory<char>> wrapper ) : m_wrapper( wrapper ) {
            #ifdef DEBUG_ALLOCATORS
            std::cout << "Instance of wrapper!" << std::endl;
            #endif
        }

        /*************************************************************************************************************
         * \brief Default destructor for AllocatorWrapper.
         */
        T * allocate( size_t n ) {
            return reinterpret_cast<T*>( m_wrapper->allocate( n * sizeof( T ) ) );
        }

        /*************************************************************************************************************
         * \brief Deallocates memory for an array of T objects.
         * \param p Pointer to the memory to be deallocated.
         * \param n Number of elements of type T to deallocate.
         * \note The pointer p is cast to char* before deallocation to match the expected
         *       type of the underlying allocator's deallocate method.
         */
        void deallocate( T* p, size_t n ) {
            m_wrapper->deallocate( (char*) p, n * sizeof( T ) );
        }

        /*************************************************************************************************************
         * \brief Returns a shared pointer to the underlying AllocatorFactory.
         */
        std::shared_ptr<AllocatorFactory<char>> getWrapper() const { return m_wrapper; }

        /*************************************************************************************************************
         * \brief Copy constructor for AllocatorWrapper.
         */
        template <typename U>
        AllocatorWrapper( AllocatorWrapper<U> const& other ) noexcept : m_wrapper ( other.m_wrapper ){}

        template <typename U> friend class AllocatorWrapper;

        /*************************************************************************************************************
         * \brief Rebinds the allocator to a different type U.
         */
        template <typename U>
        struct rebind {
            using other = AllocatorWrapper<U>;
        };
};