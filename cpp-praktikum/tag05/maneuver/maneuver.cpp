#include "maneuver.h"
#include <cmath>
#include <fstream>

Maneuver::Maneuver()
{
    bIsRunning = false;
    adWishSpeed[0] = 0.0;
    adWishSpeed[1] = 0.0;
    dMaxSpeed = 0.5;        // Hoechstens 0,5 m/s pro Rad.
    dPosDifference = 0.02; // Ein Punkt gilt bis 2 cm Abstand als erreicht.
    iter = coordList.end();
}

void Maneuver::CalcCircle(double R, double v, double T)
{
    // Vor jedem neuen Manoever die alten Punkte entfernen.
    coordList.clear();
    iter = coordList.end();
    bIsRunning = false;
    double pi = std::acos(-1.0);

    // Die Schleifengrenze entspricht genau der Vorgabe im Skript.
    // (int) schneidet den Nachkommateil der berechneten Schrittzahl ab.
    for (int counter = 1; counter < (int)((2.0 * pi) / ((v / R) * T)); counter++)
    {
        double winkel = counter * (v / R) * T;
        double x = R * std::sin(winkel);
        double y = R * (1.0 - std::cos(winkel));
        coordList.push_back(Coord(x, y, v)); // Punkt hinten an die Liste haengen.
    }
}

void Maneuver::CalcEight(double R, double v, double T)
{
    coordList.clear();
    iter = coordList.end();
    bIsRunning = false;
    double pi = std::acos(-1.0);

    // Der erste Teilkreis liegt oberhalb der x-Achse.
    for (int counter = 1; counter < (int)((2.0 * pi) / ((v / R) * T)); counter++)
    {
        double winkel = counter * (v / R) * T;
        double x = R * std::sin(winkel);
        double y = R * (1.0 - std::cos(winkel));
        coordList.push_back(Coord(x, y, v));
    }

    // Der zweite Teilkreis hat dieselben x-Werte, aber negative y-Werte.
    for (int counter = 1; counter < (int)((2.0 * pi) / ((v / R) * T)); counter++)
    {
        double winkel = counter * (v / R) * T;
        double x = R * std::sin(winkel);
        double y = -R * (1.0 - std::cos(winkel));
        coordList.push_back(Coord(x, y, v));
    }
}

void Maneuver::LogList(std::string dateiname)
{
    std::ofstream datei(dateiname);

    // Ein eigener Iterator durchlaeuft alle Punkte der Liste.
    // p->dX bedeutet: auf dX des Eintrags zugreifen, auf den p zeigt.
    for (std::list<Coord>::iterator p = coordList.begin(); p != coordList.end(); p++)
    {
        datei << p->dX << "\t" << p->dY << "\n";
    }
    datei.close();
}

bool Maneuver::isRunning()
{
    return bIsRunning;
}

void Maneuver::Start()
{
    iter = coordList.begin(); // Wieder beim ersten Punkt beginnen.
    bIsRunning = true;
}

void Maneuver::Stop()
{
    bIsRunning = false; // Den Iterator behalten, damit Fortsetzen moeglich ist.
}

void Maneuver::Proceed()
{
    bIsRunning = true;
}

void Maneuver::CalcManeuverSpeed(double x, double y, double w)
{
    // end() steht hinter dem letzten Punkt. Dort darf iter->dX nicht mehr
    // verwendet werden. Das gilt auch, wenn die Liste noch leer ist.
    if (!bIsRunning || iter == coordList.end())
    {
        adWishSpeed[0] = 0.0;
        adWishSpeed[1] = 0.0;
        Stop();
        return;
    }

    // 1. Abstand zwischen aktueller Position und Zielpunkt berechnen.
    double dx = iter->dX - x;
    double dy = iter->dY - y;
    double abstand = std::sqrt(dx * dx + dy * dy);
    if (abstand <= dPosDifference)
    {
        iter++; // Der Punkt wurde erreicht: den naechsten Punkt anfahren.
    }

    // 2. Nach dem letzten Punkt beide Sollwerte auf null setzen und stoppen.
    if (iter == coordList.end())
    {
        adWishSpeed[0] = 0.0;
        adWishSpeed[1] = 0.0;
        Stop();
        return;
    }

    // 3. Richtung zum Zielpunkt. atan2 erwartet zuerst y, dann x.
    double phi = std::atan2(iter->dY - y, iter->dX - x);

    // 4./5. Unterschied zur aktuellen Richtung auf ]-pi, pi] begrenzen.
    double differenz = phi - w;
    double pi = std::acos(-1.0);
    if (differenz > pi)
    {
        differenz = differenz - 2.0 * pi;
    }
    else if (differenz <= -pi)
    {
        differenz = differenz + 2.0 * pi;
    }

    // 6. Drehanteil: Faktor 2 laut Skript, begrenzt auf +/-0,5 m/s.
    double rotation = 2.0 * differenz;
    if (rotation > dMaxSpeed)
    {
        rotation = dMaxSpeed;
    }
    else if (rotation < -dMaxSpeed)
    {
        rotation = -dMaxSpeed;
    }

    // 7. Die Fahrgeschwindigkeit kommt aus dem Listeneintrag.
    double translation = iter->dZ;

    // 8. Die Drehung hat Vorrang. Fuer die Geradeausfahrt bleibt nur der
    // Teil von 0,5 m/s uebrig, der nicht schon fuer die Drehung gebraucht wird.
    // Das entspricht den Vorzeichenfaellen der Abbildung 34.
    double rest = dMaxSpeed - std::fabs(rotation);
    if (translation > rest)
    {
        translation = rest;
    }
    else if (translation < -rest)
    {
        translation = -rest;
    }

    // 9. Positive Rotation: rechtes Rad schneller, linkes Rad langsamer.
    adWishSpeed[0] = translation + rotation;
    adWishSpeed[1] = translation - rotation;
}

double* Maneuver::GetManeuverSpeed()
{
    return adWishSpeed;
}
