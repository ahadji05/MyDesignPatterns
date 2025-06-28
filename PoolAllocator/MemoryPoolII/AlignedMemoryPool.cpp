
#include "AlignedMemoryPool.hpp"

AlignedMemoryPool::AlignedMemoryPool( size_t numberOfBytes )
{
    // specialize implementation based on OS; Linux, Aplle, or Windows
    #if defined(__linux__) || defined(__APPLE__)
        this->m_blockSize = ( size_t ) sysconf ( _SC_PAGESIZE );
    #elif _WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        this->m_blockSize = (size_t) sysInfo.dwPageSize;
    #else
        static_assert(false, "Non-supported OS detected in AlignedMemoryPool( size_t numberOfBytes )");
    #endif

    // first we call the base-class constructor to initilize internal member variables
    this->initialize_memory_pool( numberOfBytes, this->m_blockSize );

    // then we allocate a large aligned piece of memory
    #if defined(__linux__) || defined(__APPLE__)
        if ( posix_memalign( (void**)&m_pool, this->m_blockSize, this->m_blockSize ) != 0 )
            std::runtime_error("AlignedMemoryPool: posix_memalign failed");
    #elif _WIN32
        m_pool = ( void* ) VirtualAlloc( nullptr, this->m_blockSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
        if ( !m_pool )
            std::runtime_error("AlignedMemoryPool: VirtualAlloc failed");
    #endif

    // finally we set the pointers of the blocks at fixed intervals such that every 
    // block's base address is page-aligned.
    for ( size_t i = 0; i < this->m_numberOfBlocks; ++i )
        this->m_blocks[i]->blockMemory = m_pool + ( i * this->m_blockSize );
}


AlignedMemoryPool::~AlignedMemoryPool()
{
    // free is also OS dependent; in all case set m_pool to nullptr
    #if defined(__linux__) || defined(__APPLE__)
        free( m_pool );
    #elif _WIN32
        VirtualFree(ptr, 0, MEM_RELEASE);
    #else
        static_assert(false, "Non-supported OS detected in ~AlignedMemoryPool");
    #endif
    m_pool = nullptr;
}