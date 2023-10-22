
#include <vector>
#include <memory>

#include "ProductA.hpp"
#include "ProductB.hpp"
#include "ProductC.hpp"

#include "FactoryX.hpp"
#include "FactoryY.hpp"

using ftype = std::unique_ptr<Factory>;
using ptype = std::unique_ptr<Product>;

/**
 * @brief An interface that manipulates Factories and Products. Both Products and Factories 
 * are developed (separately) based on the abstract factory desing pattern, in order to ease 
 * extensibility in terms new types: Factories and/or Products.
 * 
 * For example, a new Factory-type can be developed as a new concrete class derived from the 
 * abstract Factory class. The new factory is directly accessible from the interface. 
 * Same extensibiliy applies for Product-types.
 */
int main(){

    std::vector<std::pair<ftype,ptype>> v;
    v.emplace_back(new FactoryX(5), new ProductA(3));
    v.emplace_back(new FactoryY(1), new ProductC(2));
    v.emplace_back(new FactoryX(4), new ProductB(3));

    for (auto const& p : v){
        std::cout << "-------------------\n";
        p.first->get_factory();
        p.second->get_product();
    }

    return EXIT_SUCCESS;
}