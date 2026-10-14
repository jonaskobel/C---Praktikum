#include "pos_estimation.h"
#include <cmath>

PosEstimation::PosEstimation()
{
    Reset();
}

void PosEstimation::Reset()
{
    // Der Startpunkt wird zum Ursprung des Koordinatensystems.
    x[0] = 0.0;
    x[1] = 0.0;
    x[2] = 0.0;
    dVelAverage = 0.0;
}

void PosEstimation::PredictPosition(double rechts, double links, double T)
{
    double B = 0.23;            // Spurbreite der Simulation in Metern.
    double pi = std::acos(-1.0);

    // 1. Position mit der ALTEN mittleren Geschwindigkeit und dem ALTEN
    // Winkel berechnen. Deshalb darf dVelAverage erst unten geaendert werden.
    x[0] = x[0] + dVelAverage * T * std::cos(x[2]);
    x[1] = x[1] + dVelAverage * T * std::sin(x[2]);

    // 2. Ein schnelleres rechtes Rad dreht den Roboter nach links.
    x[2] = x[2] + T * (rechts - links) / B;

    // 3. Winkel auf das Intervall ]-pi, pi] begrenzen.
    // fmod berechnet den Rest einer Division auch fuer Kommazahlen.
    x[2] = std::fmod(x[2], 2.0 * pi);
    if (x[2] > pi)
    {
        x[2] = x[2] - 2.0 * pi;
    }
    else if (x[2] <= -pi)
    {
        x[2] = x[2] + 2.0 * pi;
    }

    // 4. Diesen Mittelwert erst beim NAECHSTEN Aufruf fuer x und y nutzen.
    dVelAverage = (rechts + links) / 2.0;
}

double* PosEstimation::GetPosition()
{
    // Der Arrayname liefert die Adresse des ersten Elements.
    return x;
}
