#include <iostream>
#include <string> 


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


// swapp letters of tow letters next to each other 
std::string swapp_letters_in_string(
    std::string word
)
{
    int len = word.size(); // Beispiel = 8
    char swapp_letter; 

    for (int i = 1; i + 1 < word.size(); i+= 2) // B:0 e:1 i:2 s:3 p:4 i:5 e:6 l:7 
    {
        
        swapp_letter = word[i]; 
        word[i] = word[i+1]; 
        word[i+1] = swapp_letter; 
    }

    return word; 
}


// remove vowels 
std::string remove_vowels_in_string(
    std::string word
)
{
    std::string vowels = "aeiouAEIOU"; 
    int pos = word.find_first_of(vowels); 

    while (pos != std::string::npos) // npos wert für nicht gefunden 
    {
        word.erase(pos, 1); 
        pos = word.find_first_of(vowels, pos); 
    }

    return word; 
}

int main()
{
    std::string word = read_text(
        "Type in word to swapp letters and erase vowels: "
    ); 

    std::string swapped_letters = swapp_letters_in_string(
        word
    ); 
    
    std::cout <<"Word with swapped letters: " << swapped_letters << '\n'; 

    std::string remove_vowels = remove_vowels_in_string(
        word
    ); 

    std::cout <<"Word with removed vowels: " << remove_vowels << '\n'; 

}