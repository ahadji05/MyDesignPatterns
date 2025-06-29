

add_library( mempool
    MemoryPool.cpp
    AlignedMemoryPool.cpp 
    ContiguousMemoryPool.cpp 
    CudaMemoryPool.cpp 
)

if(USE_CUDA)
target_link_libraries( mempool PUBLIC CUDA::cudart )
endif()
