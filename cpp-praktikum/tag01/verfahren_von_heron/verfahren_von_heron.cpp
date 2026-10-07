#include "verfahren_von_heron.h"
#include <iostream>


// read number from user input 
double read_number(
    const std::string &text
)
{
    std::cout << text; 
    double number; 

    while (true)
    {
        if (std::cin >> number)
        {
            std::string rest;  // definition von rest als string 
            std::getline(std::cin, rest);  //konsumiere den restlichen Input 

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


double heron_itr(
    double a, 
    double x0, // random start value 
    int num_of_iter // define number of iterations 
)
{
    double x_n = x0; 
    double x_n_plus_1; 
    for (int i = 0; i <= num_of_iter; ++i)
    {
        x_n_plus_1 = (x_n +(a/x_n))/2; 
        x_n = x_n_plus_1; 
    }

    return x_n_plus_1; 
}