
#include <iostream>
#include "AllocatorFactory.hpp"
#include <vector>
#include <list>
#include <map>

#define TYPE_AND_ALLOCATOR(type,name) type,AllocatorFactory<type,name>

#define HostMalloc(type) TYPE_AND_ALLOCATOR(type,AllocHostMalloc)
#define CudaPinned(type) TYPE_AND_ALLOCATOR(type,AllocHostPinned)
#define CudaDevice(type) TYPE_AND_ALLOCATOR(type,AllocDeviceCuda)

template<typename T, template<typename> class Allocator>
void compute( std::vector<T, AllocatorFactory<T,Allocator>> &vec );

void compute( std::vector<HostMalloc(float)> &vec ) {
    std::cout << "Computing on host!\n";
}

void compute( std::vector<CudaDevice(float)> &vec ) {
    std::cout << "Computing on device!\n";
}

int main( int argc, char *argv[] ){
    std::cout << "Memory Pools and Allocators" << std::endl;

    // AllocatorFactory<int,AllocHostMalloc> mallocator( AllocatorKind::HOST_MALLOC );

    // int *p = ( int * ) mallocator.allocate( 1 );

    // p[0] = 5;

    // std::cout << p[0] << std::endl;

    // mallocator.deallocate( p, 1 );

    std::vector<HostMalloc(float)> vec;
    vec.reserve(10);
    compute( vec );

    std::vector<CudaDevice(float)> vecd;
    vecd.reserve(10);
    compute( vecd );

    // std::vector<CudaDevice(float)> vec_d;

    // std::list<CudaDevice(float)> list;

    // std::map<HostMalloc(int32_t)> map;

    // auto listAllocator = list.get_allocator();

    // std::list<CudaDevice(float)> list2( listAllocator );

    // vec.reserve( 5 );

    // vec.push_back( 2 );

    // compute( vec_d );

    return EXIT_SUCCESS;
}