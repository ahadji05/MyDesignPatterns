
#include "MemoryPool.hpp"
#include <cmath>


MemoryPool::MemoryPool()
{
}


MemoryPool::~MemoryPool()
{
    for ( size_t i = 0; i < m_numberOfBlocks; ++i )
        delete m_blocks[i];

    m_blocks.clear();
    m_usedBlocks.clear();
    m_allocMap.clear();
    m_blockIndex.clear();
}


void MemoryPool::initialize_memory_pool( size_t numberOfBytes, size_t blockSize )
{
    m_blockSize = blockSize;
    m_numberOfBlocks = _num_blocks_requested( numberOfBytes );
    m_blocks.resize( m_numberOfBlocks );
    m_usedBlocks.resize( m_numberOfBlocks, false );
    m_lastFreedOrAllocBlock = 0;

    // allocate memory for the blocks and set the block-indices in the unordered_map m_blockIndex
    for ( size_t i = 0; i < m_numberOfBlocks; ++i ) {
        m_blocks[i] = new Block();
        m_blockIndex[m_blocks[i]] = i;
    }
}



size_t MemoryPool::_num_blocks_requested( size_t nBytes )
{
    return 1 + ( ( nBytes - 1 ) / m_blockSize);
}


std::pair<size_t,size_t> MemoryPool::_find_allocated_block( void *ptr )
{
    auto it = m_allocMap.find( ptr );

    #if DEBUG_MEMORY_POOL
        if ( it == m_allocMap.end() )
            throw std::runtime_error( "Pointer not found in allocation map" );
    #endif

    return it->second;
}


void MemoryPool::_release_block( Block *block )
{
    auto it = m_blockIndex.find( block );

    #if DEBUG_MEMORY_POOL
        if ( it == m_blockIndex.end() )
            throw std::runtime_error( "Block not found" );
        
        if ( m_usedBlocks.at( it->second ) == false )
            throw std::runtime_error( "It is already false" );
    #endif

    m_usedBlocks[it->second] = false;
    m_lastFreedOrAllocBlock = it->second;
}


void MemoryPool::_allocate_block( Block *block )
{
    auto it = m_blockIndex.find( block );

    #if DEBUG_MEMORY_POOL
        if ( it == m_blockIndex.end() )
            throw std::runtime_error( "Block not found" );

        if ( m_usedBlocks.at(it->second) == true )
            throw std::runtime_error( "It is already true" );
    #endif

    m_usedBlocks[it->second] = true;
    m_lastFreedOrAllocBlock = it->second;
}


void * MemoryPool::do_allocate( size_t nBytes )
{
    size_t blocksNeeded = _num_blocks_requested( nBytes );

    #ifdef DEBUG_MEMORY_POOL
        std::cout << "alloc blocks needed: " << blocksNeeded << std::endl;
    #endif

    // Find contiguous free blocks
    size_t startIdx = m_numberOfBlocks;
    size_t count = 0;

    for ( size_t i = m_lastFreedOrAllocBlock; i < m_numberOfBlocks; ++i )
        if ( !m_usedBlocks[i] ) {
            if ( count == 0 ) startIdx = i;
            count++;
            if ( count == blocksNeeded ) break;
        } else {
            count = 0;
        }

    // if did not manage to find in the range: [m_lastFreedOrAllocBlock - end]
    // try search in the range: [begin - m_lastFreedOrAllocBlock]
    if ( count < blocksNeeded )
        for ( size_t i = 0; i < m_lastFreedOrAllocBlock + blocksNeeded; ++i ) {
            if ( !m_usedBlocks[i] ) {
                if ( count == 0 ) startIdx = i;
                count++;
                if ( count == blocksNeeded ) break;
            } else {
                count = 0;
            }
        }

    if ( count < blocksNeeded ) {
    #ifdef DEBUG_MEMORY_POOL
            std::cerr << "Not enough contiguous blocks available\n";
    #endif
        return nullptr;
    }

    #ifdef DEBUG_MEMORY_POOL
        std::cout << "alloc: [ " << startIdx << " - " <<  startIdx + count - 1 << " ]" << std::endl;
    #endif

    // Mark blocks as used
    for ( size_t i = 0; i < blocksNeeded; ++i )
        _allocate_block( m_blocks[startIdx + i] );

    // Record allocation metadata
    void* userPtr = static_cast<void*>( m_blocks[startIdx]->blockMemory );
    m_allocMap[userPtr] = { startIdx, blocksNeeded };

    return userPtr;
}


void MemoryPool::do_deallocate( void *ptr )
{
#if DEBUG_MEMORY_POOL
    try {
#endif
        auto pair = _find_allocated_block( ptr );
        size_t  startIdx = pair.first;
        int64_t count  = (int64_t) pair.second;
        for ( int64_t i = (count - 1); i >= 0; --i )
            _release_block( m_blocks[startIdx + i] );
        m_allocMap.erase( ptr );
#if DEBUG_MEMORY_POOL
    std::cout << "dealloc: [ " << startIdx + count - 1 << " - " << startIdx << " ]" << std::endl;
    } catch (...) {
        std::cerr << "Attempted to deallocate invalid pointer" << std::endl;
    }
#endif
}