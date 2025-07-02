#pragma once

#include <iostream>

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
        size_t        m_numberOfAllocations;
        size_t        m_totalAllocatedMemory;

    public:
        using value_type = T;

        AllocatorKind getKind()     const { return this->m_kind;                 }
        std::string   getName()     const { return this->m_name;                 }
        size_t        getNumberOfAllocations() const { return this->m_numberOfAllocations;  }
        size_t        getTotalAllocatedMemory() const { return this->m_totalAllocatedMemory; }

        AllocatorFactory() noexcept {std::cout<<"Ctor\n";}
        ~AllocatorFactory() noexcept {std::cout<<"Dtor\n";}

        virtual T *allocate( size_t nElements ) = 0;

        virtual void deallocate( T *p, [[maybe_unused]] size_t size = 0 ) = 0;
};

template<class T> 
class AllocHostMalloc : public AllocatorFactory<T> {
    public:
        using value_type = T;

        AllocHostMalloc() noexcept { 
            std::cout << "AllocHostMalloc Ctor\n"; 
            this->m_kind = AllocatorKind::HOST_MALLOC; 
            this->m_name = "HOST_ALLOCATOR"; 
            this->m_numberOfAllocations  = 0;
            this->m_totalAllocatedMemory = 0;
        }

        ~AllocHostMalloc() = default;

        T * allocate( size_t nElements ) override {
            if ( this->m_kind != AllocatorKind::HOST_MALLOC )
                throw std::runtime_error("The allocator with name "+this->m_name+" is not compatible with AllocatorKind::HOST_MALLOC!");
            
            std::cout << "Allocating on host!\n";

            ++this->m_numberOfAllocations;
            size_t nbytes = nElements * sizeof(T);
            this->m_totalAllocatedMemory += nbytes;
            return ( T* ) malloc ( nbytes );
        }

        void deallocate( T * p, [[maybe_unused]] size_t size ) override {
            std::cout << "Deallocating on host!\n";
            free( p );
        }
};

template<class T> 
class AllocDeviceCuda : public AllocatorFactory<T> {
    public:
        using value_type = T;

        AllocDeviceCuda() noexcept { 
            std::cout << "AllocDeviceCuda Ctor\n";
            this->m_kind = AllocatorKind::DEVICE_CUDA;
            this->m_name = "DEVICE_ALLOCATOR";
            this->m_numberOfAllocations  = 0;
            this->m_totalAllocatedMemory = 0;
        }

        ~AllocDeviceCuda() = default;

        T * allocate( size_t nElements ) override {

            if ( this->m_kind != AllocatorKind::DEVICE_CUDA )
                throw std::runtime_error("The allocator with name "+this->m_name+" is not compatible with AllocatorKind::DEVICE_CUDA!");

            std::cout << "Allocating on device!\n";

            ++this->m_numberOfAllocations;
            size_t nbytes = nElements * sizeof(T);
            this->m_totalAllocatedMemory += nbytes;
            return ( T* ) malloc ( nbytes );
        }

        void deallocate( T * p, [[maybe_unused]] size_t size ) override {
            std::cout << "Deallocating on device!\n";
            free( p );
        }
};

template<class T> 
class AllocHostPinned : public AllocatorFactory<T> {
    public:
        using value_type = T;

        AllocHostPinned() noexcept {
            std::cout << "AllocHostPinned Ctor\n";
            this->m_kind = AllocatorKind::HOST_PINNED_CUDA;
            this->m_name = "PINNED_ALLOCATOR";
            this->m_numberOfAllocations  = 0;
            this->m_totalAllocatedMemory = 0;
        }
        
        ~AllocHostPinned() = default;

        T * allocate( size_t nElements ) override {

            if ( this->m_kind != AllocatorKind::HOST_PINNED_CUDA )
                throw std::runtime_error("The allocator with name "+this->m_name+" is not compatible with AllocatorKind::HOST_PINNED_CUDA!");

            std::cout << "Allocating pinned on host!\n";
           
            ++this->m_numberOfAllocations;
            size_t nbytes = nElements * sizeof(T);
            this->m_totalAllocatedMemory += nbytes;
            return ( T* ) malloc ( nbytes );

        }

        void deallocate( T * p, [[maybe_unused]] size_t size ) override {
            std::cout << "Deallocating pinned on host!\n";
            free( p );
        }
};
