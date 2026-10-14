#include "PIDController.h"
#include <fstream>
#include <iostream>

int main()
{
    // Die Parameter sind die Vorgaben der Aufgabe "Regler", Teil c.
    PIDController regler(1000.0, 10.0, 0.1, 0.5);
    std::ifstream datei("PIDControllerInput.txt");
    double soll;
    double ist;

    // Linke Spalte: Sollwert. Rechte Spalte: Istwert.
    // Die Schleife endet, wenn keine weiteren Werte eingelesen werden koennen.
    while (datei >> soll >> ist)
    {
        regler.CalculateU(soll, ist);
        std::cout << regler.GetU() << std::endl;
    }

    datei.close();
    // Die ausgegebenen Werte auf der VM mit PIDControllerVgl.txt vergleichen.
    return 0;
}
