#include "vererbung_und_ueberschreiben.h"
#include <iostream>

int main()
{
    // d) Person und Student erstellen
    Person p1("Anna", "Meyer", 35);
    Student s1("Jonas", "Kobel", 22, 123456);

    std::cout << p1.getName() << '\n'; // Anna
    std::cout << s1.getName() << '\n'; // Student Jonas

    // Geerbte und zusätzliche Methoden verwenden
    std::cout << s1.getSurname() << '\n';
    std::cout << s1.getAge() << '\n';
    std::cout << s1.getStudentID() << '\n';

    // e) Einer Person einen Student zuweisen
    Person p2("Max", "Mustermann", 40);
    p2 = s1;

    std::cout << p2.getName() << '\n';    // Jonas
    std::cout << p2.getSurname() << '\n'; // Kobel
    std::cout << p2.getAge() << '\n';     // 22

    // Nicht möglich:
    // p2.getStudentID();
    // s1 = p2;

    return 0;
}