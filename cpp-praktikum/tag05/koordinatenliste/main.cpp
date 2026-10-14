#include "../maneuver/maneuver.h"

int main()
{
    Maneuver maneuver;

    // Genau die Werte aus der Aufgabe "Koordinatenliste" verwenden.
    maneuver.CalcCircle(2.0, 0.3, 0.04);
    maneuver.LogList("LogFileCircle.txt");

    maneuver.CalcEight(5.0, 1.0, 10.0);
    maneuver.LogList("LogFileEight.txt");

    // Beide Dateien entstehen im aktuellen Arbeitsordner.
    // Auf der VM im Projekt Kapitel5_Koordinatenliste_Test pruefen.
    return 0;
}
