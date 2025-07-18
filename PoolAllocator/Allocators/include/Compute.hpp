#pragma once

#include "AllocatorFactory.hpp"

#include <vector>


/*************************************************************************************
 * \brief Main routine.
 */
int run_main_routine();

/**
 * \brief A basic example.
 */
void run_example();

/*************************************************************************************
 * \brief Compute function that does some calculations using a vector that is allocated by a generic allocator.
 */
void compute( std::vector<float, AllocatorWrapper<float>> &vec );


/*************************************************************************************
 * \brief Example of usage with STL containers: list, map, set, unordered_map, unordered_set.
 */
void exampleStlContainers();
