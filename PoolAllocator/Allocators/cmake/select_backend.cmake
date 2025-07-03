

# IF NO BACKEND WAS SELECTED RETURN WITH ERROR
# --------------------------------------------
if(USE_OPENMP)
set(BACKEND_SELECTED ON)
target_compile_definitions(main.exe PRIVATE PPT_ENABLE_OPENMP_BACKEND)
elseif(USE_CUDA)
set(BACKEND_SELECTED ON)
target_compile_definitions(main.exe PRIVATE PPT_ENABLE_CUDA_BACKEND)
else()
set(BACKEND_SELECTED OFF)
message(FATAL_ERROR "NO BACKEND WAS SPECIFIED! CHOOSE AMONG: \n -DUSE_OPENMP=ON \n -DUSE_CUDA=ON")
endif()


# LINK WITH OPENMP IF USE_OPENMP IS ON
# ------------------------------------
if(USE_OPENMP)
find_package(OpenMP)
target_link_libraries( main.exe OpenMP::OpenMP_CXX allocs )
endif()


# LINK WITH CUDA LIBRARIES IF USE_CUDA IS ON
# ------------------------------------------
if(USE_CUDA)
enable_language(CUDA)
find_package(CUDAToolkit REQUIRED)
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries( main.exe PUBLIC CUDA::cudart allocs )
endif()
