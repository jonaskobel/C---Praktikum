#include "vehicle.h"

int vehicle::next_id = 0; // mus einmalig zu beginn definiert werden 

vehicle::vehicle(color c, double p, int y)
{
    m_color = c; 
    price = p; 
    year_of_construction = y; 
    
    next_id++; // next id einmal erhöhen 

    idn = next_id; // id zuweisen
}

double vehicle::getPrice() const {
    return price;
}

int vehicle::getYearOfConstruction() const {
    return year_of_construction; 
}

int vehicle::getId() const {
    return idn; 
}


// define the methodes 
std::string vehicle::getColor() const { // const muss mitgenommen werden 
    
    switch (m_color)
    {
        case blue:
            return "blue";
        case red:
            return "red";
        case green:
            return "green"; 
        case white: 
            return "white"; 
        case black:
            return "black"; 
        
        default:
            return "unkown"; 
    }
}

bool vehicle::isOldtimer(const vehicle& v)
{
    if (v.year_of_construction < 1980)
        return true; 
    return false; 
}



