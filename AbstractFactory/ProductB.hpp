#pragma once

#include "Product.hpp"
#include <iostream>

class ProductB: public Product {

public:
    ProductB( int const& i ) { this->create_product(i); };
    ~ProductB(){}

    void create_product( int const serial_no ) { this->m_serial_no = serial_no; }
    void get_product() const { std::cout << "Product B: " << m_serial_no << "\n";  }
};