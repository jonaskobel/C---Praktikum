#include <iostream>


void swap(
    int& uebergabe_1, 
    int& uebergabe_2
)
{
    int temp = uebergabe_1; 
    uebergabe_1 = uebergabe_2; 
    uebergabe_2 = temp; 

}


int main()
{
    int zahl1 = 4; 
    int zahl2 = 5; 

    std::cout << "Zahl 1: " << zahl1 << '\n'; 
    std::cout << "Zahl 2: " << zahl2 << '\n';

    swap(zahl1, zahl2); 

    std::cout << "swap function: \n"; 
    std::cout << "Zahl 1: " << zahl1 << '\n'; 
    std::cout << "Zahl 2: " << zahl2 << '\n';

}