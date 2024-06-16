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


template<typename T>
int generate_2d_dense( std::vector<std::array<T,2>> &data, size_t nx, size_t ny, T dx, T dy, T xmin, T ymin, bool verbose = false ){

    // make sure no values existed before in the vector of data.
    data.clear();

    // loop over x,y coordinates using x as the fastes index and grid a rectangular grid of coordinates.
    for ( size_t iy(0); iy <  ny; ++iy ){
        T y = ymin + iy * dy;
        for( size_t ix(0); ix < nx; ++ix ){
            T x = xmin + ix * dx;
            std::array<T,2> coord = { x, y };
            data.push_back( coord );
            if(verbose)
                std::cout << "("<<coord[0]<<","<<coord[1]<<")"<<"  ";
        }
        if(verbose)
            std::cout << std::endl;
    }

    return 0;
}
