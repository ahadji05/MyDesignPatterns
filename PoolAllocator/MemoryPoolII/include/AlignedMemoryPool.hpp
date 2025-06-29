#pragma once

#include "MemoryPool.hpp"

#if defined(__linux__) || defined(__APPLE__)
#include "unistd.h"
#elif _WIN32
#include <windows.h>
#endif

class AlignedMemoryPool : public MemoryPool {
    private:
        char * m_pool = nullptr;

    public:
        AlignedMemoryPool( size_t numberOfBytes );
        ~AlignedMemoryPool();
        void *allocate( size_t nBytes );
        void  deallocate( void *p );
};
