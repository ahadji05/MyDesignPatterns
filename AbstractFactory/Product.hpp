#pragma once

class Product {
protected:
    int m_serial_no;

public:
    Product(){}
    ~Product(){}

    virtual void create_product( int const serial_no ) = 0;
    virtual void get_product() const = 0;
};