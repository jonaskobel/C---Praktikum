#include "maya_numbers.h"
#include <iostream>
#include <vector>

int read_number(
    const std::string &text
)

{
    int number;
    std::cout << text;

    while (true)
    {

        if (std::cin >> number) // es werden nur numerische werte akzeptiert 
        {
            std::string rest; 
            std::getline(std::cin, rest); // nimmt die restlichen Zeichen aus dem Stream 

            if (rest.find_last_not_of(" \t\r") == std::string::npos)
            {
                return number; 
            }
        }
        else
        {
            std::cin.clear(); 
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            ); 
        }
        
        std::cout << "unvalid imput \n"; 
    }
}


std::vector<int> calc_maya_number(
    int number
)
{
    std::vector<int> digits; 

    if (number == 0)
    {
        digits.push_back(0); 
        return digits; 
    }  

    while (true)
    {
        digits.push_back(number % 20);
        number /=20; // same as number = number / 20

        if (number == 0)
        {
            std::reverse(digits.begin(), digits.end()); 
            return digits; 
        }
    }
}