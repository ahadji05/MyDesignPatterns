
#include <iostream>

#include "generate.hpp"
#include "kdtree.hpp"

using dtype = float;
constexpr int dim = 2;

std::ostream& operator<<(std::ostream & o, std::array<dtype,dim> &value){
    o << "( ";
    for( size_t i(0); i < dim-1; ++i )
        o << value[i] << ", ";
    o << value[dim-1] << " )";
    return o;
}

int main(){
    int err;

    std::cout << "Program started..." << std::endl;

    int numData = 100000;
    std::vector<std::array<dtype,dim>> data;

    err = generate_random<dtype,dim>(numData, data);
    if(err){ std::cout << "error in call to generate_random<dtype,dim>(numData, data, true);" << std::endl; return EXIT_FAILURE; }

    node_t<dtype,dim> *root = build_kdtree<dtype,dim>( data );

    // std::cout << "root node: " << root->m_key.m_value[0] << std::endl;
    // print_kdtree<dtype,dim>(root);


    int nDepths;

    std::array<dtype,dim> queryPoint0 = { +2, 0 };
    key<dtype,dim> k0 = ann_search<dtype,dim>(root,queryPoint0,nDepths);
    std::cout << "ANN of point " << queryPoint0 << " is point " << data[k0.m_id] << " - ";
    std::cout << "found after " << nDepths << " queries" << std::endl;

    std::array<dtype,dim> queryPoint = { -2, +2 };
    key<dtype,dim> k = ann_search<dtype,dim>(root,queryPoint,nDepths);
    std::cout << "ANN of point " << queryPoint << " is point " << data[k.m_id] << " - ";
    std::cout << "found after " << nDepths << " queries" << std::endl;

    destroy_kdtree(root);

    return EXIT_SUCCESS;
}
