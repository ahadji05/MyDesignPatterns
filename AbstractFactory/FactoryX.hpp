#pragma once

#include "Factory.hpp"

class FactoryX : public Factory{

    public:
        FactoryX(int const& s){ this->create_factory(s); }
        ~FactoryX(){}

        void create_factory( int const& s ) { this->m_factory_no = s; };
        void get_factory() const { std::cout << "FactoryX: " << this->m_factory_no << "\n"; }
};