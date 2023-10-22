#pragma once

#include <iostream>
#include <string>

class Factory {
    protected:
        int m_factory_no;
    
    public:
        Factory(){}
        ~Factory(){}

        virtual void create_factory( int const&) = 0;
        virtual void get_factory() const = 0;
};