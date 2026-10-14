#ifndef POLYMORPHIE_H
#define POLYMORPHIE_H


class A
{
public:
    virtual void g();
    virtual void f();
    void h();
    virtual void i();
};

class B : public A
{
public:
    void g();
    void f();
};

class C : public B
{
public:
    void g();
    void h();
    void i();
};

#endif
