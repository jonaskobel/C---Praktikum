#include "squares.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const square& s)
{
    os << "Quadrat: Kantenlänge=" << s.getEdgeLength()
       << ", Fläche=" << s.getArea()
       << ", Umfang=" << s.getPerimeter()
       << ".";

    return os;
}

int main()
{
    square s1(10); 
    square s2(4); 

    square squares[2] = {s1, s2}; 

    for (int i = 0; i < 2; i++)
    {
        std::cout << "Kantenlänge: " << squares[i].getEdgeLength() << '\n'; 
        std::cout << "Fläche: " << squares[i].getArea() << '\n'; 
        std::cout << "Umfang: " << squares[i].getPerimeter() << '\n'; 
    }

    // kopieren Konstruktor testen 
    square s3(s1); 

    square sum = s1 + s2; 
    square diff = s1 - s2; 

    // e) Quadrate direkt mit std::cout ausgeben
    std::cout << "s1: " << s1 << '\n';
    std::cout << "s2: " << s2 << '\n';
    std::cout << "Kopie von s1: " << s3 << '\n';
    std::cout << "Summe: " << sum << '\n';
    std::cout << "Differenz: " << diff << '\n';

    return 0;
}