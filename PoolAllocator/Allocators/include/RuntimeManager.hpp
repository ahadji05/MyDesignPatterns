#pragma once

#include "AllocatorWrapper.hpp"
#include "AllocHostMalloc.hpp"
#include "AllocHostPinned.hpp"
#include "AllocDeviceCuda.hpp"

#include <memory>
#include <map>

/*************************************************************************************************************
 * \brief Alias typename for map of global allocators.
 */
using MapAllocators_t = std::map< std::string, std::shared_ptr< AllocatorFactory<char> > >;


/*************************************************************************************************************
 * \brief Runtime manager with a global map of allocators.
 */
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


/*************************************************************************************************************
 * \brief Routine that initializes the RunTimeManager's global-allocators.
 */
void initialize_runtime_manager();


/*************************************************************************************************************
 * \brief Print allocator's stats.
 */
void print_allocators_statistics();