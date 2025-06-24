
#include "AllocatorFactory.hpp"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <numeric>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <set>

/**
 * Compute function that does some calculations using a vector that is allocated by a generic allocator.
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
//--------------------------------------------------------------------------------------------------------

/**
 * Runtime manager with a global map of allocators.
 */
using MapAllocators_t = std::map< std::string, std::shared_ptr< AllocatorFactory<char> > >;

class RunTimeManager {
    private:
        static MapAllocators_t globalAllocators;

        friend void initialize_runtime_manager();
        friend void print_allocators_statistics();

    public:
        static std::shared_ptr<AllocatorFactory<char>> getGlobalAllocator( std::string const& name ) {            
 
            auto it = globalAllocators.find( name );
            
            if ( it == globalAllocators.end() )
                throw std::runtime_error("Runtime manager encountered a request for an invalid allocator with name: "+name);

            return it->second;
        }
};

MapAllocators_t RunTimeManager::globalAllocators = MapAllocators_t();
//--------------------------------------------------------------------------------------------------------

/**
 * Routine that initializes the RunTimeManager's global-allocators.
 */
void initialize_runtime_manager() {
    /**
     * \brief Here we create a map of allocators with three different names: HOST, DEVICE, PINNED.
     * Each allocator is a restricted constant pointer and it is accessible by a specified name
     * from the list above. 
     */
    std::cout << "Runtime Manager: initializing global allocators..." << std::endl;
    RunTimeManager::globalAllocators.insert( { "HOST",   std::make_shared<AllocHostMalloc<char>>( AllocHostMalloc<char>() ) } );
    RunTimeManager::globalAllocators.insert( { "DEVICE", std::make_shared<AllocDeviceCuda<char>>( AllocDeviceCuda<char>() ) } );
    RunTimeManager::globalAllocators.insert( { "PINNED", std::make_shared<AllocHostPinned<char>>( AllocHostPinned<char>() ) } );
}
//--------------------------------------------------------------------------------------------------------

/**
 * Print allocator's stats.
 */
void print_allocators_statistics() {
    std::cout << "Allocator's stats:" << std::endl;
    std::cout << " "<< RunTimeManager::getGlobalAllocator("HOST")->getName()   << ":"
              << "\n -number of calls       : " << RunTimeManager::getGlobalAllocator("HOST")->getNumberOfAllocations()
              << ",\n -total bytes allocated : " << RunTimeManager::getGlobalAllocator("HOST")->getTotalAllocatedMemory() << std::endl;
    std::cout << " "<< RunTimeManager::getGlobalAllocator("DEVICE")->getName() << ":"
              << "\n -number of calls       : " << RunTimeManager::getGlobalAllocator("DEVICE")->getNumberOfAllocations()
              << ",\n -total bytes allocated : " << RunTimeManager::getGlobalAllocator("DEVICE")->getTotalAllocatedMemory() << std::endl;
    std::cout << " "<< RunTimeManager::getGlobalAllocator("PINNED")->getName() << ":"
              << "\n -number of calls       : " << RunTimeManager::getGlobalAllocator("PINNED")->getNumberOfAllocations()
              << ",\n -total bytes allocated : " << RunTimeManager::getGlobalAllocator("PINNED")->getTotalAllocatedMemory() << std::endl;
}
//--------------------------------------------------------------------------------------------------------

/**
 * Main routine.
 */
int run_main_routine(){

    // pass the global HOST allocator to a vector which eventually stores 2 float values: { 4, 3 }
    std::vector<float, AllocatorWrapper<float>> vec( AllocatorWrapper<float>{ RunTimeManager::getGlobalAllocator( "HOST" ) } );
    vec.reserve( 2 ); // alloc-host +1
    vec.push_back(4);
    vec.push_back(3);
    std::cout << "1--------------------------\n";
    compute( vec ); // compute on host = 7
    std::cout << "1--------------------------\n";

    // pass the global DEVICE allocator to a vector which eventually stores 3 float values: { 5, 1, 3 }
    std::vector<float, AllocatorWrapper<float>> vec_d( AllocatorWrapper<float>{ RunTimeManager::getGlobalAllocator( "DEVICE" ) } );
    vec_d.push_back(5); // alloc-device +1
    vec_d.push_back(1); // alloc-device +1
    vec_d.push_back(3); // alloc-device +1
    std::cout << "2--------------------------\n";
    compute( vec_d ); // compute on device = 9
    std::cout << "2--------------------------\n";

    // get HOST allocator from the vector that was initialized with the "HOST" global allocator
    auto hostAllocator = vec.get_allocator();
    float *p = ( float * ) hostAllocator.allocate( 100 ); // alloc-host +1
    hostAllocator.deallocate( p );

    return 0;
}

/**
 * Main program. 
 * Returns with EXIT_SUCCESS after a call to the main routine.
 * If the main routine returns with error, or throws an exception, the main program returns with EXIT_FAILURE.
 */
int main( int argc, char *argv[] ) {

    // initialize the runtime manager with the global allocators: "HOST", "DEVICE", "PINNED"
    initialize_runtime_manager();

    try {
        int error = run_main_routine();
        if ( error ) return EXIT_FAILURE;
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    // print allocator's stats: number of calls, and number of bytes allocated
    print_allocators_statistics();


    /*************************************************************************************
     * Example of usage with STL containers: list, map, set, unordered_map, unordered_set.
     */
    std::list<
                int, 
                AllocatorWrapper<int>
                > aList( AllocatorWrapper<int>{ RunTimeManager::getGlobalAllocator("HOST") } );

    std::map<int,
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

    return EXIT_SUCCESS;
}
