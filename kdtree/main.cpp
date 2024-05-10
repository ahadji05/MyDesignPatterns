
#include <iostream>

#include "generate.hpp"
#include "kdtree.hpp"

using dtype = float;
constexpr int dim = 1;

int main(){
    int err;

    std::cout << "Program started..." << std::endl;

    int numData = 17;
    std::vector<std::array<dtype,dim>> data;

    err = generate_random<dtype,dim>(numData, data, true);
    if(err){ std::cout << "error in call to generate_random<dtype,dim>(numData, data, true);" << std::endl; return EXIT_FAILURE; }

    node_t<dtype,dim> *root = build_kdtree<dtype,dim>( data );

    std::cout << "root node: " << root->m_key.m_value[0] << std::endl;
    print_kdtree<dtype,dim>(root);

    destroy_kdtree(root);

    return 0;
}
