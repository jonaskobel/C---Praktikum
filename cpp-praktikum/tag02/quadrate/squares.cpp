#include "squares.h"

// erster Konstrukor
square::square(double l) 
    : edge_length(l) // da als const double edge_length definiert 
{
}

// zweiter Konstruktor
square::square(const square& s) 
    : edge_length(s.edge_length)
{
}

// Methoden 
double square::getEdgeLength() const {
    return edge_length; 
}

double square::getArea() const {
    return edge_length*edge_length; 
}

double square::getPerimeter() const {
    return 4*edge_length; 
}


// operatoren definiren: 
square operator+(const square& a, const square& b)
{
    return square(a.getEdgeLength() + b.getEdgeLength()); 
}

square operator-(const square& a, const square& b)
{
    return square(a.getEdgeLength() - b.getEdgeLength()); 
}
