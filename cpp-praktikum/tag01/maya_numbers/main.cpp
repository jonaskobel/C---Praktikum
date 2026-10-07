#include "maya_numbers.h"
#include <iostream>

int main()
{
    std::cout << "Calculate Maya Numbers" << '\n';

    int number = read_number(
        "Type in a Number to convert to Maya Numbers: "
    ); 

    std::vector<int> result = calc_maya_number(number); 

    for (const int& digit : result) // const bedeutet nur lese Zugriff
    {
        std::cout << digit << ' '; 
    }

    std::cout << '\n'; 

    double test; 


}