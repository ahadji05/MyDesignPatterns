
#include <iostream>

#include "generate.hpp"
#include "kdtree.hpp"

using dtype = float;
constexpr int dim = 1;

int main(){
    int err;

    std::cout << "Program started..." << std::endl;

    int numData = 6;
    std::vector<std::array<dtype,dim>> data;

    err = generate_random<dtype,dim>(numData, data, true);
    if(err){ std::cout << "error in call to generate_random<dtype,dim>(numData, data, true);" << std::endl; return EXIT_FAILURE; }

    node_t<dtype,dim> *root;
    err = make_kdtree<dtype,dim>(root, data);

    return 0;
}