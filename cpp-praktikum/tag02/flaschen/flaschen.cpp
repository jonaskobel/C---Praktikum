#include "flaschen.h"
#include <iostream>
#include <string>

Flasche::Flasche() 
{
    dVolume = 0.0; 
    sMaterial = "unkown"; 
}
double Flasche::getdVolume() const
{
    return dVolume; 
}

void Flasche::setdVolumn(double volume)
{
    dVolume = volume; 
}

std::string Flasche::getsMaterial() const
{
    return sMaterial; 
}

void Flasche::setsMaterial(std::string material)
{
    sMaterial = material; 
}

void Flasche::printFlasche() const
{
    std::cout << dVolume; 
    std::cout << sMaterial; 
}

void Flasche::adoptFlasche(const Flasche& other)
{
    dVolume = other.getdVolume(); 
    sMaterial = other.getsMaterial(); 
}