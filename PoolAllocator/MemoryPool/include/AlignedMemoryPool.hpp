#pragma once

#include "MemoryPool.hpp"

#if defined(__linux__) || defined(__APPLE__)
#include "unistd.h"
#elif _WIN32
#include <windows.h>
#endif

/*************************************************************************************************************
 * @brief Implements a memory pool for aligned memory management.
 * This class allocates memory using aligned allocation and deallocates it using aligned deallocation.
 * For Linux and macOS, it uses `posix_memalign`, and for Windows, it uses `virtualAlloc` and `VirtualFree`.
 */
class AlignedMemoryPool : public MemoryPool {
    private:
        char * m_pool = nullptr;

    public:
        AlignedMemoryPool( size_t numberOfBytes );
        ~AlignedMemoryPool();
        void *allocate( size_t nBytes );
        void  deallocate( void *p );
};
