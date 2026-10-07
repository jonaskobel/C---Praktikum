#include <iostream>
#include <cstdlib>
#include <ctime>

// swap bigger and smaller number: 
void swap(
    int& bigger_number, 
    int& smaller_number
)
{
    int tmp = bigger_number; 
    bigger_number = smaller_number; 
    smaller_number = tmp; 
}

// man kann nicht den gesamten array übergeben:
// Bei einem eingebauten C++-Array wird dabei aber automatisch die Adresse 
// des ersten Elements übergeben. Das Array wird nicht vollständig kopiert.
// Deshalb nimmt die Funktion einen Pointer entgegen:
void bubble_sort(
    int* numbers, 
    int len
)
{ 
    // über den pointer kann man die zahlen aus dem array laden 
    for (int i = 0; i < len-1; ++i)
    {
        for (int j = 0; j < len-1-i; ++j)
        {
            if (numbers[j] > numbers[j+1])
            {
                swap(numbers[j], numbers[j+1]); 
                // als pointer: swap(&numbers[j], &numbers[j+1]); 
            }
        }
    }
}

int* create_random_array(int len)
{
    srand(time(nullptr)); 

    int* numbers = new int[len]; // hier wird die adresse + array erzeugt 

    for (int i = 0; i < len; ++i)
    {
        numbers[i] = std::rand() % 101; 
    }

    return numbers; // die adresse mnuss zurückgeben werden, weil sie noch nicht in der main funtkion sichtbar ist 
}

int main()
{
    int len = 32; 
    int* numbers = create_random_array(len); 

    bubble_sort(numbers, len);

    for (int i = 0; i < len; ++i)
    {
        std::cout << numbers[i] << ' '; 
    }

    std::cout << '\n'; 

    delete[] numbers; 
}
