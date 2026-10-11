#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>

// define coord 
struct coord{
    float x; 
    float y; 
    coord(float x_in, float y_in){
        x = x_in; 
        y = y_in; 
    }
}; 

void printlist(const std::list<coord>* koordinaten){

    // iterator 
    std::list<coord>::const_iterator myiter; 

    for (myiter = koordinaten->begin(); myiter != koordinaten->end(); myiter++){
        std::cout << "X: " << myiter ->x << ", Y: " << myiter ->y << '\n'; 
    }

}

void delcoords(std::list<coord>& koordinaten){

    std::list<coord>::const_iterator myiter; 

    for (myiter = koordinaten.begin(); myiter != koordinaten.end(); ){

        if (myiter->x < myiter->y)
        {
            myiter = koordinaten.erase(myiter); 
        }
        else
        {
            ++myiter; 
        }
    }
}

// d) Funktionen verwenden
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::list<coord> koordinaten;

    // Zehn zufällige Punkte: Werte von 0 bis 10 in 0,1-Schritten
    for (int i = 0; i < 10; ++i) {
        float x = (std::rand() % 101) / 10.0f;
        float y = (std::rand() % 101) / 10.0f;

        koordinaten.push_back(coord(x, y));
    }

    std::cout << "Urspruengliche Liste:\n";
    printlist(&koordinaten);

    // Eigenständige Kopie der gesamten Liste
    std::list<coord> backup = koordinaten;

    delcoords(koordinaten);

    std::cout << "\nNach dem Loeschen:\n";
    printlist(&koordinaten);

    std::cout << "\nBackup:\n";
    printlist(&backup);

    return 0;
}