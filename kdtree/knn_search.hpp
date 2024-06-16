#pragma once

// C++ program to find groups of unknown Points using K nearest neighbour algorithm.
#include <bits/stdc++.h>
#include <vector>

template<typename T, int N>
struct Point{
    std::array<T,N> coord;  // Co-ordinate of point
    T distance;             // Distance from test point
    Point( std::array<T,N> p ) : coord(p), distance(0) {}
};

// Used to sort an array of points by increasing
// order of distance
// bool comparison(Point<float,1> a, Point<float,1> b){ return (a.distance < b.distance); }
bool comparison(Point<float,2> a, Point<float,2> b){ return (a.distance < b.distance); }
// bool comparison(Point<float,3> a, Point<float,3> b){ return (a.distance < b.distance); }
// bool comparison(Point<float,4> a, Point<float,4> b){ return (a.distance < b.distance); }
// bool comparison(Point<float,5> a, Point<float,5> b){ return (a.distance < b.distance); }

template<typename T, int N>
std::vector<Point<T,N>> knn_search( Point<T,N> queryPoint, int k, std::vector<Point<T,N>> vec ) {

    // Fill distances of all points from queryPoint.
    for ( size_t i = 0; i < vec.size(); ++i ){
        vec[i].distance = 0;
        for ( size_t dim = 0; dim < N; ++dim )
            vec[i].distance += std::pow(vec[i].coord[dim] - queryPoint.coord[dim],2);
        vec[i].distance = std::sqrt(vec[i].distance);
    }

    // Sort the Points by distance from queryPoint.
    std::sort(vec.begin(), vec.end(), comparison);

    // Select and return the k nearest-neighbors.
    std::vector<Point<T,N>> knn;
    for( int i(0); i < k; ++i )
        knn.push_back(vec[i]);

    return knn;
}

template<typename T, int N>
std::vector<Point<T,N>> knn_search( std::array<T,N> const& coord, int k, std::vector<std::array<T,N>> const& data ) {
    std::vector<Point<T,N>> vec;
    for( size_t i(0); i < data.size(); ++i )
        vec.push_back( Point<T,N>(data[i]) );
    
    Point<T,N> queryPoint = Point<T,N>( coord );

    return knn_search( queryPoint, k, vec );
}
