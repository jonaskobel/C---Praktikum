#include "vehicle.h"
#include <iostream>

int main()
{
    vehicle v1(vehicle::red, 15000, 1979); 
    vehicle v2(vehicle::blue, 22000, 1980); 

    std::cout << "Fahrzeug 1:\n";
    std::cout << "ID: " << v1.getId() << '\n';
    std::cout << "Farbe: " << v1.getColor() << '\n';
    std::cout << "Preis: " << v1.getPrice() << " Euro\n";
    std::cout << "Baujahr: " << v1.getYearOfConstruction() << '\n';
    std::cout << "Oldtimer: " << vehicle::isOldtimer(v1) << "\n\n";

    std::cout << "Fahrzeug 2:\n";
    std::cout << "ID: " << v2.getId() << '\n';
    std::cout << "Farbe: " << v2.getColor() << '\n';
    std::cout << "Preis: " << v2.getPrice() << " Euro\n";
    std::cout << "Baujahr: " << v2.getYearOfConstruction() << '\n';
    std::cout << "Oldtimer: " << vehicle::isOldtimer(v2) << '\n';

    return 0;
}
