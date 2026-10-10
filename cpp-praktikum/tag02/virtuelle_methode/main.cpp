#include "vererbung_und_ueberschreiben.h"
#include <iostream>

int main()
{
    // pointer auf Objekt erstelle:
    // Person* p1 .. erstellt nur eine Adresse, nicht aber ein Objekt, dafür entweder m1 oder mit new ein dynamisches Objekt erstellen 
    // m1: 
    // Student s1(..); 
    // Person* p1 = &s1; 
    // m2: 
    // Person* p1 = new Student("Jonas", "Kobel", 22, 123456); 
    // Person* p2 = new Student("Anna", "Mayer", 21, 4235345); 

    // Der Zeigertyp ist Person*.
    // Das tatsächlich erzeugte Objekt ist jeweils ein Student.
    // new erstellt das Objekt und liefert seine Adresse.

    Person* p1; 
    Person* p2; 

    p1 = new Student("Jonas", "Kobel", 22, 123456); 
    p2 = new Student("Anna", "Mayer", 21, 4235345); 

    std::cout << p1->getName() << " " << p1->getSurname() << ", " << p1->getAge() << " Jahre\n"; 
    std::cout << p2->getName() << " " << p2->getSurname() << ", " << p2->getAge() << " Jahre\n";
    
    // dynamische Objekte müssen wieder zerstört werden 
    delete p1; 
    delete p2; 
}