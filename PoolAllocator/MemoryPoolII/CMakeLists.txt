cmake_minimum_required(VERSION 3.22)

project( MemoryPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


# SPECIFY BACKEND OPTIONS
# -----------------------
# set(USE_OPENMP ON )
set(USE_CUDA ON)
set(CMAKE_CUDA_ARCHITECTURES "86")
set(CMAKE_BUILD_TYPE "Release")
add_compile_definitions(DEBUG_MEMORY_POOL)

# SPECIFY EXECUTABLE NAME
# -----------------------
add_executable( main.exe main.cu )


# SPECIFY BACKEND
# ---------------
include(cmake/select_backend.cmake)


# SPECIFY THE DIRECTORY TO HEADER FILES
# -------------------------------------
include_directories(include)


# SPECIFY THE SOURCE FILES REQUIRED TO BUILD THE TARGET EXECUTABLE
# ----------------------------------------------------------------
add_subdirectory(source)
