
#include <iostream>

#include "generate.hpp"
#include "kdtree.hpp"
#include "knn_search.hpp"

using dtype = float;
constexpr int dim = 2;

std::ostream& operator<<(std::ostream & o, std::array<dtype,dim> &value){
    o << "( ";
    for( size_t i(0); i < dim-1; ++i )
        o << value[i] << ", ";
    o << value[dim-1] << " )";
    return o;
}


std::ostream& operator<<(std::ostream & o, Point<dtype,dim> &value){
    o << "( ";
    for( size_t i(0); i < dim-1; ++i )
        o << value.coord[i] << ", ";
    o << value.coord[dim-1] << " ) -> ";
    o << value.distance;
    return o;
}

int main(){
    int err;

    std::cout << "Program started..." << std::endl;

    int ny = 201;
    int nx = 201;
    dtype ymin = 0;
    dtype xmin = 0;
    dtype dy = 25;
    dtype dx = 25;
    std::vector<std::array<dtype,dim>> data;

    err = generate_2d_dense<dtype>(data, nx, ny, dx, dy, xmin, ymin);
    if(err){ std::cout << "error in call to generate_2d_dense<dtype,dim>(...);" << std::endl; return EXIT_FAILURE; }

    node_t<dtype,dim> *root = build_kdtree<dtype,dim>( data );

    int nDepths;
    std::array<dtype,dim> queryPoint = { 500, 4400 };
    key<dtype,dim> queryKey = ann_search<dtype,dim>(root->m_left->m_right->m_left,queryPoint,nDepths);
    std::cout << "ANN of point " << queryPoint << " is point " << data[queryKey.m_id] << " - ";
    std::cout << "found after " << nDepths << " queries at distance: " << compare<dtype,dim>(queryPoint,data[queryKey.m_id]);
    std::cout << std::endl;

    destroy_kdtree(root);

    int k = 10;
    std::vector<Point<dtype,dim>> knn = knn_search<dtype,dim>( queryPoint, k, data );
    std::cout << std::endl;
    std::cout << "the " << k << "-nn of point " << queryPoint << " are:" << std::endl;
    int pos = 0;
    for( auto e : knn ){
        std::cout << pos << ": " << e << "\n";
        ++pos;
    }

    return EXIT_SUCCESS;
}