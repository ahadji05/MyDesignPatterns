

add_library( mempool
    MemoryPool.cpp
    AlignedMemoryPool.cpp 
    HostMemoryPool.cpp 
    CudaMemoryPool.cpp 
    saxpy_kernel.cu
)

if(USE_CUDA)
target_link_libraries( mempool PUBLIC CUDA::cudart )
endif()
