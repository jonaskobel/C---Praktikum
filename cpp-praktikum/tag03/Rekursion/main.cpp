#include <iostream>

double factorailIter(int number){

    if (number < 0){
        return 0; 
    }

    // 5*4*3*2*1
    double factorial = 1; 
    for (int i = 1; i <= number; i++)
    {
        factorial = factorial * i; 
    }

    return factorial; 
}


double factorialRec(int number){

    if (number < 0){
        return 0; 
    }

    if (number == 0){
        return 1; 
    }

    return number * factorialRec(number -1); 
}


int main(){

    int number; 

    std::cout << "Type in number: "; 
    std::cin >> number; 

    std::cout << "iterativ: " << factorailIter(number) << '\n'; 
    std::cout << "rekursiv: " << factorialRec(number) << '\n'; 

}
