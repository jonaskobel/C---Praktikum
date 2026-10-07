#include "palindrome.h"
#include <iostream>

int main()
{
    std::string user_input = read_text(
        "Enter a word to check whether it is a palindrome: "
    );

    bool is_pali = is_palindrome(
        user_input
    ); 

  
    if (is_pali)
    {
        std::cout << "The word: " << user_input << " is a palindrom" << '\n'; 
    }
    else 
    {
        std::cout << "The word: " << user_input << " is NOT a palindrom" << '\n'; 
    }


}