
# Add all .cpp files in this directory to the library
file(GLOB ALLOCS_SOURCES *.cpp)

# Create a static library named 'allocs' from the source files
add_library(allocs 
    STATIC 
        ${ALLOCS_SOURCES}
)

# Specify the include directories for the 'allocs' target
target_include_directories(allocs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if(USE_CUDA)
    target_link_libraries(allocs 
        PUBLIC 
            CUDA::cudart
)
endif()
