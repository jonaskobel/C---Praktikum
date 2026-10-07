#include "verfahren_von_heron.h"
#include <iostream>


int main()
{
    std::cout << "Calculate the square root using Heron’s method: " << '\n'; 

    double number = read_number(
        "Type in a number: "
    ); 

    double random_start_value = 1; 
    int num_of_iterations = 10; 

    double result = heron_itr(
        number, 
        random_start_value, 
        num_of_iterations
    );

    std::cout << "Result after " << num_of_iterations << " iterations: " << result << '\n'; 

}


