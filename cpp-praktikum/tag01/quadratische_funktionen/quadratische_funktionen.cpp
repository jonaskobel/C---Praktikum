#include <iostream>
#include <cmath>

// read number
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


bool calc_quad_fct_sol(
    double a, 
    double b,
    double c, 
    double* x1, 
    double* x2
)
{
    double d; 
    d = b*b - 4*a*c; 

    if (d < 0)
    {
        return false;  // keine reelle Lösung 
    }

    // ein pointer wird benötigt um mehr als ein Wert zurückzugeben
    *x1 = (-b + std::sqrt(d))/(2*a); 
    *x2 = (-b - std::sqrt(d))/(2*a); 

    return true; 
}


int main()
{
    std::cout << "Calculate the solution of a quadratic function ax^2 + bx + c = 0 :" << '\n'; 

    double a = read_number(
        "Type in a: "
    ); 

    double b = read_number(
        "Type in b: "
    );

    double c = read_number(
        "Type in c: "
    );

    double x1; 
    double x2; 

    bool sol_exists = calc_quad_fct_sol(a,b,c, &x1, &x2); 

    if (sol_exists)
    {
        std::cout << "X1: " << x1 << '\n'; 
        std::cout << "X2: " << x2 << '\n'; 
    }
    else 
    {
        std::cout << "No real solution. \n"; 
    }

    return 0; 
}
