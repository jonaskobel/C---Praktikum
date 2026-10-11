#include <iostream>
#include <exception>
#include <fstream>


class notfileException : public std::exception 
{
    public: 
        const char* what() const noexcept override {
            return "file does not exists"; 
        }
}; 


notfileException noFileError; 

void read_file(std::string filename){

    std::string one_line; 
    std::fstream file; 
    file.open(filename, std::ios::in); //nur Lesezugriff

    if (file.fail())
    {
        throw noFileError; // falls failbit gesetzt, wirf das Objekt vom Type nofileExeptions
    }

    while (std::getline(file, one_line)){
        std::cout << one_line << '\n'; 
    } 
    
    file.close(); 
}

int main(){

    std::string filename; 
    std::cout << "filename: "; 
    std::getline(std::cin, filename); 

    try {
        read_file(filename); 
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n'; 
    }

}