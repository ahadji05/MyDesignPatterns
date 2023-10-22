#pragma once

#include "Factory.hpp"

class FactoryY : public Factory {

    public:
        FactoryY(int const& s){ this->create_factory(s); }
        ~FactoryY(){}
        
        void create_factory( int const& s ) { this->m_factory_no = s; };
        void get_factory() const { std::cout << "FactoryY: " << this->m_factory_no << "\n"; }
};