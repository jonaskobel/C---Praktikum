#ifndef FLASCHEN_H
#define FLASCHEN_H
#include <string>

class Flasche
{
private: 
    double dVolume; 
    std::string sMaterial; 

public:
    Flasche(); // standart constructor
    double getdVolume() const; 
    void setdVolumn(double dVolume); 

    std::string getsMaterial() const; 
    void setsMaterial(std::string sMaterial);

    void printFlasche() const; 
    void adoptFlasche(const Flasche& other);
}; 

#endif
