#ifndef SQUARES_S
#define SQUARES_S

class square
{
private:
   const double edge_length; // damit nichtmehr veränderbar

public:
    //Konstrukor
    square(double length); 
    square(const square& s); 

    // define methods 
    double getEdgeLength() const; 
    double getArea() const; 
    double getPerimeter() const; 
};

square operator+(const square& a, const square& b); 
square operator-(const square& a, const square& b); 


#endif