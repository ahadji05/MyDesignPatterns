#pragma once

#include <iostream>
#include <cstddef>
#include <memory>

enum class AllocatorKind {
    HOST_MALLOC,
    HOST_PINNED_CUDA,
    DEVICE_CUDA
};

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

        AllocatorKind getKind()     const { return this->m_kind;                 }
        std::string   getName()     const { return this->m_name;                 }
        size_t        getNumberOfAllocations() const { return this->m_numberOfAllocations;  }
        size_t        getNumberOfDeallocations() const { return this->m_numberOfDeallocations;  }
        size_t        getTotalAllocatedMemory() const { return this->m_totalAllocatedMemory; }
        size_t        getTotalDeallocatedMemory() const { return this->m_totalDellocatedMemory; }
        size_t        getCurrentMemoryUsage() const { return this->m_currentMemoryUsage; }

        AllocatorFactory()  noexcept { 
            #ifdef DEBUG_ALLOCATORS
            std::cout<<"Ctor\n";
            #endif 
        }

        ~AllocatorFactory() noexcept {
            #ifdef DEBUG_ALLOCATORS
            std::cout<<"Dtor\n";
            #endif
        }

        virtual T *allocate( size_t nElements ) = 0;

        virtual void deallocate( T *p, size_t nElements ) = 0;
};

template<class T> 
class AllocHostMalloc : public AllocatorFactory<T> {
    public:
        using value_type = T;

        AllocHostMalloc() noexcept {
            #ifdef DEBUG_ALLOCATORS
            std::cout << "AllocHostMalloc Ctor\n"; 
            #endif
            this->m_kind = AllocatorKind::HOST_MALLOC; 
            this->m_name = "HOST_ALLOCATOR"; 
            this->m_numberOfAllocations  = 0;
            this->m_numberOfDeallocations = 0;
            this->m_totalAllocatedMemory = 0;
            this->m_totalDellocatedMemory = 0;
            this->m_currentMemoryUsage = 0;
        }

        ~AllocHostMalloc() = default;

        T * allocate( size_t nElements ) override {
            if ( this->m_kind != AllocatorKind::HOST_MALLOC )
                throw std::runtime_error("The allocator with name "+this->m_name+" is not compatible with AllocatorKind::HOST_MALLOC!");
            
            #ifdef DEBUG_ALLOCATORS
            std::cout << "Allocating on host! ( " << nElements * sizeof( T ) << " ) bytes \n";
            #endif

            ++this->m_numberOfAllocations;
            size_t nbytes = nElements * sizeof( T );
            this->m_totalAllocatedMemory += nbytes;
            this->m_currentMemoryUsage += nbytes;
            return ( T* ) malloc ( nbytes );
        }

        void deallocate( T * p, size_t nElements ) override {
            ++this->m_numberOfDeallocations;
            this->m_totalDellocatedMemory += ( nElements * sizeof( T ) );
            this->m_currentMemoryUsage -= ( nElements * sizeof( T ) );

            #ifdef DEBUG_ALLOCATORS
            std::cout << "Deallocating on host! ( " << nElements * sizeof( T ) << " ) bytes \n";
            #endif
            free( p );
        }
};

template<class T> 
class AllocDeviceCuda : public AllocatorFactory<T> {
    public:
        using value_type = T;

        AllocDeviceCuda() noexcept {
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

        ~AllocDeviceCuda() = default;

        T * allocate( size_t nElements ) override {

            if ( this->m_kind != AllocatorKind::DEVICE_CUDA )
                throw std::runtime_error("The allocator with name "+this->m_name+" is not compatible with AllocatorKind::DEVICE_CUDA!");

            #ifdef DEBUG_ALLOCATORS
            std::cout << "Allocating on device!\n";
            #endif

            ++this->m_numberOfAllocations;
            size_t nbytes = nElements * sizeof( T );
            this->m_totalAllocatedMemory += nbytes;
            this->m_currentMemoryUsage += nbytes;
            return ( T* ) malloc ( nbytes );
        }

        void deallocate( T * p, size_t nElements ) override {
            ++this->m_numberOfDeallocations;
            this->m_totalDellocatedMemory += ( nElements * sizeof( T ) );
            this->m_currentMemoryUsage -= ( nElements * sizeof( T ) );

            #ifdef DEBUG_ALLOCATORS
            std::cout << "Deallocating on device!\n";
            #endif
            free( p );
        }
};

template<class T> 
class AllocHostPinned : public AllocatorFactory<T> {
    public:
        using value_type = T;

        AllocHostPinned() noexcept {
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
        
        ~AllocHostPinned() = default;

        T * allocate( size_t nElements ) override {

            if ( this->m_kind != AllocatorKind::HOST_PINNED_CUDA )
                throw std::runtime_error("The allocator with name "+this->m_name+" is not compatible with AllocatorKind::HOST_PINNED_CUDA!");

            #ifdef DEBUG_ALLOCATORS
            std::cout << "Allocating pinned on host!\n";
            #endif

            ++this->m_numberOfAllocations;
            size_t nbytes = nElements * sizeof( T );
            this->m_totalAllocatedMemory += nbytes;
            this->m_currentMemoryUsage += nbytes;
            return ( T* ) malloc ( nbytes );
        }

        void deallocate( T * p, size_t nElements ) override {
            ++this->m_numberOfDeallocations;
            this->m_totalDellocatedMemory += ( nElements * sizeof( T ) );
            this->m_currentMemoryUsage -= ( nElements * sizeof( T ) );

            #ifdef DEBUG_ALLOCATORS
            std::cout << "Deallocating pinned on host!\n";
            #endif
            free( p );
        }
};

template <typename T>
class AllocatorWrapper {
    private:
        std::shared_ptr<AllocatorFactory<char>> m_wrapper;

    public:
        using value_type = T;

        /**
         * \brief Constructor.
         */
        AllocatorWrapper( std::shared_ptr<AllocatorFactory<char>> wrapper ) : m_wrapper( wrapper ) {
            #ifdef DEBUG_ALLOCATORS
            std::cout << "Instance of wrapper!" << std::endl;
            #endif
        }

        /**
         * Interface function: allocate.
         */
        T* allocate( size_t n ) {
            return reinterpret_cast<T*>( m_wrapper->allocate( n * sizeof( T ) ) );
        }

        /**
         * Interface function deallocate.
         */
        void deallocate( T* p, size_t n ) {
            m_wrapper->deallocate( (char*) p, n * sizeof( T ) );
        }

        /**
         *  Returns the underlying shared-allocator wrapper.
         */
        std::shared_ptr<AllocatorFactory<char>> getWrapper() const { return m_wrapper; }

        /**
         * \brief Copy-Constructor.
         * The copy-constructor is part of the Allocator Rebind Mechanism, which
         * allows to copy allocators between different types during internal STL operations, such as resize, insert, etc.
         */
        template <typename U>
        AllocatorWrapper( AllocatorWrapper<U> const& other ) noexcept : m_wrapper ( other.m_wrapper ){}

        /* this is needed to enable access to private members of the class through the public interface, e.g., copy-constructor */
        template <typename U> friend class AllocatorWrapper;

        /**
         * \brief: STL allocators often define the following rebind structure to let containers know how to produce an Allocator<U> from Allocator<T>. 
         * 
         * \NOTE: It is not needed in C++11 or later versions, because this is the default implementation of: rebind::other.
         * Although, it is still useful to have it for compatibility with pre-C++11 code and libraries.
         */
        template <typename U>
        struct rebind {
            using other = AllocatorWrapper<U>;
        };
};