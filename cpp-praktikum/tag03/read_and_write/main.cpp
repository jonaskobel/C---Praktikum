#include <fstream>
#include <string>
#include <iostream>

// a
void read_file(std::string filename){

    std::string one_line; 
    std::fstream file; 
    file.open(filename, std::ios::in); //nur Lesezugriff


    while (std::getline(file, one_line)){
        std::cout << one_line << '\n'; 
    } 
    
    file.close(); 
}

void write_file(std::string filename){

    std::string input; 
    std::fstream file; 
    file.open(filename, std::ios::out | std::ios::app);

    std::cout << "Text input; quit with exit. \n"; 

    while (std::getline(std::cin, input)){
        if (input == "exit"){
            break; 
        }
        file << input << '\n'; 
    }
    file.close(); 
}

int main(){

    std::string filename; 
    std::cout << "filename: "; 
    std::getline(std::cin, filename);
    
    std::cout << "\nContent on file: \n"; 
    read_file(filename); 

    std::cout << "append: y/n: "; 
    std::string append; 
    std::getline(std::cin, append); 

    if (append == "y"){
        write_file(filename); 
    }
    read_file(filename); 

}


