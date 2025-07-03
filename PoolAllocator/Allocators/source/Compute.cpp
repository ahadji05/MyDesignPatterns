
#include "RuntimeManager.hpp"
#include "Compute.hpp"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>


/**
 * 
 */
int run_main_routine(){

    // initialize the runtime manager with the global allocators: "HOST", "DEVICE", "PINNED"
    initialize_runtime_manager();

    run_example();

    // print allocator's stats: number of calls, and number of bytes allocated
    print_allocators_statistics();

    return 0;
}


/**
 * 
 */
void run_example() {

    // get access to global host and device allocators
    auto hostAllocator   = RunTimeManager::getGlobalAllocator( "HOST" );
    auto deviceAllocator = RunTimeManager::getGlobalAllocator( "DEVICE" );

    // pass the global HOST allocator to a vector which eventually stores 2 float values: { 4, 3 }
    std::vector<float, AllocatorWrapper<float>> vec( AllocatorWrapper<float>{ hostAllocator } );
    vec.reserve( 2 ); // alloc-host +1
    vec.push_back(4);
    vec.push_back(3);
    std::cout << "1--------------------------\n";
    compute( vec ); // compute on host = 7
    std::cout << "HOST: current memory usage " << hostAllocator->getCurrentMemoryUsage() << " bytes" << std::endl;
    std::cout << "1--------------------------\n";

    // pass the global DEVICE allocator to a vector which eventually stores 3 float values: { 5, 1, 3 }
    std::vector<float, AllocatorWrapper<float>> vec_d( AllocatorWrapper<float>{ deviceAllocator } );
    // vec_d.push_back(5); // alloc-device +1
    // vec_d.push_back(1); // alloc-device +1
    // vec_d.push_back(3); // alloc-device +1
    // std::cout << "2--------------------------\n";
    // compute( vec_d ); // compute on device = 9
    // std::cout << "DEVICE: current memory usage " << deviceAllocator->getCurrentMemoryUsage() << " bytes" << std::endl;
    // std::cout << "2--------------------------\n";

    // get HOST allocator from the vector that was initialized with the "HOST" global allocator
    float *p = ( float * ) vec.get_allocator().allocate( 100 ); // alloc-host +1
    vec.get_allocator().deallocate( p, 100 );

    // use the HOST allocator again to allocate integers
    int *pInt = (int *) hostAllocator->allocate( 50 ); // alloc-host +1
    hostAllocator->deallocate( (char*)pInt, 50 );
}


/**
 * 
 */
void compute( std::vector<float, AllocatorWrapper<float>> &vec ) {

    switch ( vec.get_allocator().getWrapper()->getKind() ) {
        case AllocatorKind::HOST_MALLOC:
            std::cout << "Compute on host: " << std::accumulate( vec.begin(), vec.end(), 0 ) << std::endl;
            break;

        case AllocatorKind::DEVICE_CUDA:
            std::cout << "Compute on device: " << std::accumulate( vec.begin(), vec.end(), 0 ) << std::endl;
            break;

        case AllocatorKind::HOST_PINNED_CUDA:
            std::cout << "Compute on pinned-host memory: " << std::accumulate( vec.begin(), vec.end(), 0 ) << std::endl;
            break;
        
        default:
            std::cout << "Unsupported allocator kind!" << std::endl;
    }
}


/**
 * 
 */
void exampleStlContainers() {
    std::list<
            int, 
            AllocatorWrapper<int>
            > aList( AllocatorWrapper<int>{ RunTimeManager::getGlobalAllocator("HOST") } );
            // aList.push_front(6);

    std::map<
                int,
                char, 
                AllocatorWrapper<char>
                > aMap( AllocatorWrapper<char>{ RunTimeManager::getGlobalAllocator("DEVICE") } );

    std::set<
            double, 
            int,  
            AllocatorWrapper<double>
            > aSet( AllocatorWrapper<double>{ RunTimeManager::getGlobalAllocator("HOST") } );

    std::unordered_map<
                        int, 
                        char, 
                        std::hash<int>, 
                        std::equal_to<int>,
                        AllocatorWrapper<std::pair<const int, char>>
                        > anUnorderedMap( AllocatorWrapper<std::pair<const int,char>>{ RunTimeManager::getGlobalAllocator("PINNED") } );

    std::unordered_set<
                        float,
                        std::hash<float>,
                        std::equal_to<float>,
                        AllocatorWrapper<float>
                        > anUnorderedSet( AllocatorWrapper<float>{ RunTimeManager::getGlobalAllocator("DEVICE") } );

}
