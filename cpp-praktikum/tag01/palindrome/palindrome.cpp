#include "palindrome.h"
#include <iostream>
#include <string>
#include <cctype>


// read text from user input 
std::string read_text(
    const std::string &text
)
{
    std::cout << text; 
    std::string user_input; 

    while (true)
    {
        if (!(std::cin >> user_input))
        {
            return "";
        }

        bool has_digit = false; 

        for (char c : user_input)
        {
            if (std::isdigit(static_cast<unsigned char>(c)))
            {
                has_digit = true; 
                break; 
            }
        }

        if (!has_digit)
        {
            return user_input; 
        }

        std::cout << "unvalid imput \n"; 
    }

    return ""; // Stream geschlossen oder fehlerhaft 
}

bool is_palindrome(
    std::string word
)
{
    int len = word.size(); 

    // für wörter mit eine geraden Zahl an Buchstaben

    for (int i = 0; i <= len/2; i ++)   // 6 Buchstaben 1<>6, 2<>5, 3<>4 
                                        // 7 Buchstaben 1<>7, 2<>6, 3<>5, 
    {
        if (word[i] != word[len-1-i])
        {
            return false; 
        }
    }

    return true; 

}