#pragma once

#include "Product.hpp"
#include <iostream>

class ProductA: public Product {

public:
    ProductA( int const& i ) { this->create_product(i); };
    ~ProductA(){}

    void create_product( int const serial_no ) { this->m_serial_no = serial_no; }
    void get_product() const { std::cout << "Product A: " << m_serial_no << "\n";  }
};