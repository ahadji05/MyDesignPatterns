
#include "AllocatorFactory.hpp"

#include <iostream>
#include <numeric>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>

/**
 * Compute function that does some calculations using a vector that is allocated by a generic allocator.
 */
template<template<typename> class Allocator>
void compute( std::vector<float, Allocator<float>> &vec );

template<>
void compute( std::vector<float, AllocHostMalloc<float>> &vec ) {
    std::cout << "Compute on host: " << std::accumulate( vec.begin(), vec.end(), 0 ) << std::endl;
}

template<>
void compute( std::vector<float, AllocDeviceCuda<float>> &vec ) {
    std::cout << "Compute on device: " << std::accumulate( vec.begin(), vec.end(), 0 ) << std::endl;
}

template<>
void compute( std::vector<float, AllocHostPinned<float>> &vec ) {
    std::cout << "Compute on host over pinned memory: " << std::accumulate( vec.begin(), vec.end(), 0 ) << std::endl;
}
//--------------------------------------------------------------------------------------------------------


/**
 * Runtime manager with a global map of allocators.
 */
using MapAllocators_t = std::map< std::string, std::unique_ptr< AllocatorFactory<float> > >;

class RunTimeManager {
    private:
        static MapAllocators_t globalAllocators;

        friend void initialize_runtime_manager();

    public:
        static AllocatorFactory<float> *const getGlobalAllocator( std::string const& name ){            
            
            auto it = globalAllocators.find( name );
            
            if ( it == globalAllocators.end() )
                throw std::runtime_error("Runtime manager encountered a request for an invalid allocator with name: "+name);

            return it->second.get();
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
    RunTimeManager::globalAllocators.insert( { "HOST",   std::make_unique<AllocHostMalloc<float>>( AllocHostMalloc<float>() ) } );
    RunTimeManager::globalAllocators.insert( { "DEVICE", std::make_unique<AllocDeviceCuda<float>>( AllocDeviceCuda<float>() ) } );
    RunTimeManager::globalAllocators.insert( { "PINNED", std::make_unique<AllocHostPinned<float>>( AllocHostPinned<float>() ) } );
}
//--------------------------------------------------------------------------------------------------------


/**
 * Main routine.
 */
int run_main_routine(){

    // the runtime manager initializes the globale allocators
    initialize_runtime_manager();

    // allocate and deallocate memory using the global HOST allocator
    float *p = ( float *) RunTimeManager::getGlobalAllocator( "HOST" )->allocate( 100 ); // alloc +1
    RunTimeManager::getGlobalAllocator( "HOST" )->deallocate( ( float* ) p );

    // pass the global HOST allocator to a vector which eventually stores 2 float values: {4,3}
    std::cout << "0--------------------------\n";
    /* this line is a bit ridiculus, but without the cast the Factory under runtime manager does not return the correct unique-poiner type */
    std::vector<float, AllocHostMalloc<float>> vec( *static_cast<AllocHostMalloc<float>*>( RunTimeManager::getGlobalAllocator( "HOST" ) ) );
    vec.reserve( 2 ); // alloc +1
    vec.push_back(4);
    vec.push_back(3);
    std::cout << "0--------------------------\n";

    // compute on host
    std::cout << "1--------------------------\n";
    compute( vec );
    std::cout << "1--------------------------\n";

    // get the allocator from the vector and print the number of calls, which is the total number of allocations we did during the whole program.
    std::cout << "2--------------------------\n";
    auto newAlloc = vec.get_allocator();
    std::cout << newAlloc.getName() << " , calls: " << newAlloc.getNumberOfAllocations() << ", total memory: " << newAlloc.getTotalAllocatedMemory() << std::endl;
    std::cout << "2--------------------------\n";

    return 0;
}

/**
 * Main program. 
 * Returns with EXIT_SUCCESS after a call to the main routine.
 * If the main routine returns with error, or throws an exception, the main program returns with EXIT_FAILURE.
 */
int main( int argc, char *argv[] ) {

    try {
        int error = run_main_routine();
        if ( error ) return EXIT_FAILURE;
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
