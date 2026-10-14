#ifndef POLYMORPHIE_H
#define POLYMORPHIE_H

#include <iostream>

// Listing 67, Skript Seite 74.
// Wie im Skript stehen die kurzen Methoden direkt in der Headerdatei.
// Deshalb brauchen wir hier keine zusaetzliche polymorphie.cpp.

// A ist die Elternklasse. Sie hat die Methoden g, f, h und i.
// g, f und i sind virtuell: Bei einem Aufruf zaehlt der tatsaechliche
// Objekttyp. h ist dagegen NICHT virtuell.
class A
{
public:
    virtual void g()
    {
        std::cout << "A.g\n";
        f();
    }

    virtual void f()
    {
        std::cout << "A.f\n";
        h(); // Hier wird immer A::h aufgerufen, weil h nicht virtuell ist.
    }

    void h()
    {
        std::cout << "A.h\n";
        i(); // i ist virtuell: Bei einem C-Objekt wird hier C::i aufgerufen.
    }

    virtual void i()
    {
        std::cout << "A.i\n";
    }
};

// B erbt von A und ueberschreibt g und f.
// Diese Methoden bleiben virtuell, auch ohne ein erneutes "virtual".
// h und i werden von A geerbt.
class B : public A
{
public:
    void g()
    {
        std::cout << "B.g\n";
        f();
    }

    void f()
    {
        std::cout << "B.f\n";
        A::f(); // Ausdruecklich die Version von A aufrufen.
        h();    // Danach noch einmal das von A geerbte h aufrufen.
    }
};

// C erbt von B und damit auch von A.
// C ueberschreibt g und i. f wird unveraendert von B geerbt.
// Das eigene h verdeckt A::h, macht h aber nicht virtuell.
class C : public B
{
public:
    void g()
    {
        std::cout << "C.g\n";
        f(); // C hat kein eigenes f, deshalb wird B::f aufgerufen.
    }

    void h()
    {
        std::cout << "C.h\n";
        i();
    }

    void i()
    {
        std::cout << "C.i\n";
    }
};

#endif
