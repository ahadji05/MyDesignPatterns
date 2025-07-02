
__host__ __device__ inline void saxpy_kernel( int i, float a, float const* __restrict__ const x, float * __restrict__ const y ){
    y[i] = a * x[i] + y[i];
}

void saxpy_cpu( int n, float a, float const* __restrict__ const x, float * __restrict__ const y ){
    for ( int i = 0; i < n; ++i )
        saxpy_kernel(i,a,x,y);
}

__global__ void saxpy_gpu( int n, float a, float const* __restrict__ const x, float * __restrict__ const y ) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if ( i >= n ) return;
    saxpy_kernel(i,a,x,y);
}
