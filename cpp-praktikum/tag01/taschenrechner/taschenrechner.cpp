#include "taschenrechner.h"
#include <iostream>

double read_number(
    const std::string &text
)
{
    double number; // define number 

    while (true)
    {
        std::cout << text;  // print the text

        if (std::cin >> number) // es werden nur numerische Werte akzeptiert 
        {
            std::string rest; 
            std::getline(std::cin, rest); 

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

char read_expression(
    const std::string &text
)
{
    char expression; 

    while (true)
    {
        std::cout << text; 
        std::cin >> expression; 

        if (
            expression == '+' ||
            expression == '-' ||
            expression == '*' ||
            expression == '/'
        )
        {
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            ); 
            return expression; 
        }

        std::cout << "unvalide expression \n"; 
        std::cin.clear(); 
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        ); 
    }
    std::cout << text; 
    
    std::cin >> expression; 
    return expression; 
}

double calculate(
    double a,
    double b,
    char expression)
{
    switch (expression) // switch by expression
    {
    case '+': // case definition
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0) // if switch
        {
            throw std::invalid_argument("Division by zero is not allowed");
        }
        return a / b;
    default: 
        throw std::invalid_argument("unvalide input"); 
    }
}