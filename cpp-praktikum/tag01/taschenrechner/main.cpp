#include "taschenrechner.h"
#include <iostream>

int main() // beim beenden gibt das Programma automatische eine ganze zahl zurück
{
    std::cout << "Calculator: " << '\n'; 
    
    double input_a = read_number(
        "Type in first Number: "
    );
    char expression = read_expression(
        "Type in '+,-,*,/': "
    ); 
    double input_b = read_number(
        "Type in second Number: "
    );

    double result = calculate(
        input_a, 
        input_b, 
        expression
    ); 

    std::cout << "Result: " << input_a << " " << expression << " " << input_b << " = "<< result << '\n'; 
    
    return 0; // main gibt automatisch 0 zurück
}