#include <iostream>
#include "flaschen.h"

int main()
{
    Flasche flasche1; 

    // test get methods 
    std::cout << flasche1.getdVolume(); 
    std::cout << flasche1.getsMaterial(); 

    // test set methods
    flasche1.setdVolumn(0.5); 
    flasche1.setsMaterial("glas"); 

    // test print method
    flasche1.printFlasche(); 

    Flasche flasche2; 

    flasche2.adoptFlasche(flasche1); 
    flasche2.printFlasche();

}