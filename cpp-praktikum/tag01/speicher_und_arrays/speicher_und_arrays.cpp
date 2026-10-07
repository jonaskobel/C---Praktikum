#include <iostream>
#include <cstdlib>
#include <ctime>

// a)
// Bei der anlage von lokalen Variablen (Stack) muss bereits zur Übersetzungszeit 
// die größe des Arrays feststehen. 
// für int iStack[100000000]; bei int mit 4byte je Zahle würde das ungefähr 400 MB 
// Speicher sofort verbrauchen. Für den normalerweise deutlich kleineren Stack ist das zu viel

// der vorgesehene Lösungsansatz ist ein dynamischer Speicher: 

int main()
{
    srand(time(nullptr)); 

    int len = 100000; 
    int* numbers = new int[len]; // legt ein dynamisches array mit 100000 ganzzahligen Zahlen an

    for (int i = 0; i < len; ++i)
    {
        numbers[i] = std::rand() % 101; // zahl zwischen 0 und 100 
    }

    // count how many can be devided by 13 without rest 
    int counter = 0; 

    for (int i = 0; i < len; ++i)
    {
        if (numbers[i] % 13 == 0 )
        {
            ++counter; 
        }
    }

    std::cout << " Can be devided by 13: " << counter << '\n'; 

    delete[] numbers; // Speicher des arrays wieder freigeben 
}