#include "polymorphie.h"
#include <iostream>

void A::g()
{
    std::cout << "A.g\n";
    f();
}

void A::f()
{
    std::cout << "A.f\n";
    h(); 
}

void A::h()
{
    std::cout << "A.h\n";
    i(); 
}

void A::i()
{
    std::cout << "A.i\n";
}

void B::g()
{
    std::cout << "B.g\n";
    f();
}

void B::f()
{
    std::cout << "B.f\n";
    A::f(); 
    h();   
}

void C::g()
{
    std::cout << "C.g\n";
    f(); 
}

void C::h()
{
    std::cout << "C.h\n";
    i();
}

void C::i()
{
    std::cout << "C.i\n";
}
