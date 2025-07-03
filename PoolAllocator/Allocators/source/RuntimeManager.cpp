
#include "RuntimeManager.hpp"

/*************************************************************************************************************
 * Define global allocators.
 */
MapAllocators_t RunTimeManager::globalAllocators = MapAllocators_t();


/*************************************************************************************************************
 * ....
 */
void initialize_runtime_manager() {
    std::cout << "Runtime Manager: initializing global allocators..." << std::endl;
    RunTimeManager::globalAllocators.insert( { "HOST",   std::make_shared<AllocHostMalloc<char>>( AllocHostMalloc<char>() ) } );
    RunTimeManager::globalAllocators.insert( { "DEVICE", std::make_shared<AllocDeviceCuda<char>>( AllocDeviceCuda<char>() ) } );
    RunTimeManager::globalAllocators.insert( { "PINNED", std::make_shared<AllocHostPinned<char>>( AllocHostPinned<char>() ) } );
}


/*************************************************************************************************************
 * Here we create a map of allocators with three different names: HOST, DEVICE, PINNED.
 * Each allocator is a restricted constant pointer and it is accessible by a specified name
 * from the list above. 
 */
void print_allocators_statistics() {
    std::cout << "Allocator's stats:" << std::endl;
    std::cout << " "<< RunTimeManager::getGlobalAllocator("HOST")->getName()   << ":"
              << "\n -number of allocations     : " << RunTimeManager::getGlobalAllocator("HOST")->getNumberOfAllocations()
              << "\n -total bytes allocated     : " << RunTimeManager::getGlobalAllocator("HOST")->getTotalAllocatedMemory() 
              << "\n -number of deallocations   : " << RunTimeManager::getGlobalAllocator("HOST")->getNumberOfDeallocations()
              << "\n -total bytes deallocated   : " << RunTimeManager::getGlobalAllocator("HOST")->getTotalDeallocatedMemory()
              << std::endl;
    std::cout << " "<< RunTimeManager::getGlobalAllocator("DEVICE")->getName() << ":"
              << "\n -number of allocations     : " << RunTimeManager::getGlobalAllocator("DEVICE")->getNumberOfAllocations()
              << "\n -total bytes allocated     : " << RunTimeManager::getGlobalAllocator("DEVICE")->getTotalAllocatedMemory() 
              << "\n -number of deallocations   : " << RunTimeManager::getGlobalAllocator("DEVICE")->getNumberOfDeallocations()
              << "\n -total bytes deallocated   : " << RunTimeManager::getGlobalAllocator("DEVICE")->getTotalDeallocatedMemory()
              << std::endl;
    std::cout << " "<< RunTimeManager::getGlobalAllocator("PINNED")->getName() << ":"
              << "\n -number of allocations     : " << RunTimeManager::getGlobalAllocator("PINNED")->getNumberOfAllocations()
              << "\n -total bytes allocated     : " << RunTimeManager::getGlobalAllocator("PINNED")->getTotalAllocatedMemory() 
              << "\n -number of deallocations   : " << RunTimeManager::getGlobalAllocator("PINNED")->getNumberOfDeallocations()
              << "\n -total bytes deallocated   : " << RunTimeManager::getGlobalAllocator("PINNED")->getTotalDeallocatedMemory()
              << std::endl;
}
