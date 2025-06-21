#pragma once

#include <iostream>

enum class AllocatorKind {
    HOST_MALLOC,
    HOST_PINNED_CUDA,
    DEVICE_CUDA
};

template<class T> 
class AllocHostMalloc{
    public:
        using value_type = T;
        AllocHostMalloc() = default;
        ~AllocHostMalloc() = default;

        static void allocate( T **p, size_t nElems ){
            std::cout << "Allocating on host!\n";
            *p = ( T* ) malloc ( nElems * sizeof(T) );
        }

        static void deallocate( T * p ){
            std::cout << "Deallocating on host!\n";
            free( p );
        }
};

template<class T> 
class AllocDeviceCuda{
    public:
        using value_type = T;
        AllocDeviceCuda() = default;
        ~AllocDeviceCuda() = default;

        static void allocate( T **p, size_t nElems ){
            std::cout << "Allocating on Device!\n";
            *p = ( T* ) malloc ( nElems * sizeof(T) );
        }

        static void deallocate( T * p ){
            std::cout << "Deallocating on Device!\n";
            free( p );
        }
};

template<class T> 
class AllocHostPinned{
    public:
        using value_type = T;
        AllocHostPinned() = default;
        ~AllocHostPinned() = default;

        static void allocate( T **p, size_t nElems ){
            std::cout << "Allocating pinned host!\n";
            *p = ( T* ) malloc ( nElems * sizeof(T) );
        }

        static void deallocate( T * p ){
            std::cout << "Deallocating pinned host!\n";
            free( p );
        }
};

template<typename T, template<typename> class Kind>
class AllocatorFactory {
    private:
        AllocatorKind m_kind;
    public:
        using value_type = T;

        template<typename U> struct rebind { using other = AllocatorFactory<U,Kind>; };
    
        AllocatorFactory()  = default;

        ~AllocatorFactory() = default;

        template<typename U> AllocatorFactory(const AllocatorFactory<U, Kind>&) noexcept {}

        AllocatorFactory( AllocatorKind kind ) noexcept;

        T *allocate( size_t nElements );

        void deallocate( T *p, [[maybe_unused]] size_t );
};

template<typename T, template<typename> class Kind>
AllocatorFactory<T,Kind>::AllocatorFactory( AllocatorKind kind ) noexcept {
    m_kind = kind;
}

template<typename T, template<typename> class Kind>
T * AllocatorFactory<T,Kind>::allocate( size_t nElements ) {
    T *p;
    Kind<T>::allocate( &p, nElements );
    return p;
}


template<typename T, template<typename> class Kind>
void AllocatorFactory<T,Kind>::deallocate( T *p, [[maybe_unused]] size_t ) {
    Kind<T>::deallocate( p );
}