#pragma once

#include "Product.hpp"
#include <iostream>

class ProductC: public Product {

public:
    ProductC( int const& i ) { this->create_product(i); };
    ~ProductC(){}

    void create_product( int const serial_no ) { this->m_serial_no = serial_no; }
    void get_product() const { std::cout << "Product C: " << m_serial_no << "\n";  }
};