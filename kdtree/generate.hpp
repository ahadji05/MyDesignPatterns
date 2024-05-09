#pragma once

#include <array>
#include <vector>
#include <random>
#include <iostream>

template<typename T>
double format_decimals(T value, int decimal_places) {
    const T multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}

template<typename T, int N>
int generate_random( size_t numData, std::vector<std::array<T,N>> &data, bool verbose = false ){
    std::random_device dev;
    std::mt19937 rng(dev());
    double mean = 0;
    double stddeviation = 10;
    std::normal_distribution<double> normal_dist(mean, stddeviation);

    for(size_t indexData(0); indexData < numData; ++indexData ){
        std::array<T,N> tempArray;
        if ( verbose )
            std::cout << indexData << ":   ";
        for( int indexValue(0); indexValue < N; ++indexValue ){
            double value = normal_dist(rng);
            tempArray[indexValue] = static_cast<T>( format_decimals(value,2) );
            if( verbose ){
                if( tempArray[indexValue] > 0 )
                    std::cout << "+";
                std::cout << tempArray[indexValue] << "      ";
            }
        }
        if( verbose )
            std::cout << std::endl;
        data.emplace_back( tempArray );
    }

    return 0;
}
