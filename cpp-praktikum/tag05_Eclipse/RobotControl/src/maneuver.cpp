#include "maneuver.h"
#include <cmath>
#include <fstream>

Maneuver::Maneuver()
{
    blsRunning = false;

    adWishSpeed[0] = 0.0;
    adWishSpeed[1] = 0.0;

    dMaxSpeed = 0.5;
    dPosDifference = 0.02;

    iter = CoordList.end();
}

void Maneuver::CalcCircle(double dRadius, double dSpeed, double dTimestep)
{
    CoordList.clear();
    blsRunning = false;

    for (int counter = 1;
         counter < (int)((2 * M_PI) / ((dSpeed / dRadius) * dTimestep));
         counter++)
    {
        double dX = dRadius * sin(counter * (dSpeed / dRadius) * dTimestep);
        double dY = dRadius * (1 - cos(counter * (dSpeed / dRadius) * dTimestep));

        CoordList.push_back({dX, dY, dSpeed});
    }

    iter = CoordList.begin();
}

void Maneuver::CalcEight(double dRadius, double dSpeed, double dTimestep)
{
    CoordList.clear();
    blsRunning = false;

    // Erster Teilkreis
    for (int counter = 1;
         counter < (int)((2 * M_PI) / ((dSpeed / dRadius) * dTimestep));
         counter++)
    {
        double dX = dRadius * sin(counter * (dSpeed / dRadius) * dTimestep);
        double dY = dRadius * (1 - cos(counter * (dSpeed / dRadius) * dTimestep));

        CoordList.push_back({dX, dY, dSpeed});
    }

    // Zweiter Teilkreis
    for (int counter = 1;
         counter < (int)((2 * M_PI) / ((dSpeed / dRadius) * dTimestep));
         counter++)
    {
        double dX = dRadius * sin(counter * (dSpeed / dRadius) * dTimestep);
        double dY = -dRadius * (1 - cos(counter * (dSpeed / dRadius) * dTimestep));

        CoordList.push_back({dX, dY, dSpeed});
    }

    iter = CoordList.begin();
}

void Maneuver::LogList(std::string sDatei)
{
    std::ofstream file(sDatei);

    for (const Coord& punkt : CoordList)
    {
        file << punkt.dX << "\t" << punkt.dY << "\n";
    }
}

bool Maneuver::isRunning()
{
    return blsRunning;
}

void Maneuver::Start()
{
    iter = CoordList.begin();
    blsRunning = true;
}

void Maneuver::Stop()
{
    blsRunning = false;
}

void Maneuver::Proceed()
{
    blsRunning = true;
}

void Maneuver::CalcManeuverSpeed(double dX, double dY, double dW)
{
    // 1. Positionsvergleich
    if (iter != CoordList.end())
    {
        double dDeltaX = iter->dX - dX;
        double dDeltaY = iter->dY - dY;

        double dDistance = sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY);

        if (dDistance <= dPosDifference)
        {
            iter++;
        }
    }

    // 2. Listenende überprüfen
    if (iter == CoordList.end())
    {
        adWishSpeed[0] = 0.0;
        adWishSpeed[1] = 0.0;

        Stop();
        return;
    }

    // 3. Winkel zur Sollposition
    double dPhi = atan2(iter->dY - dY, iter->dX - dX);

    // 4. Winkeldifferenz
    double dDeltaPhi = dPhi - dW;

    // 5. Winkeldifferenz begrenzen
    if (dDeltaPhi > M_PI)
    {
        dDeltaPhi -= 2 * M_PI;
    }
    else if (dDeltaPhi <= -M_PI)
    {
        dDeltaPhi += 2 * M_PI;
    }

    // 6. Rotationsanteil bestimmen und begrenzen
    double dRot = 2.0 * dDeltaPhi;

    if (dRot > dMaxSpeed)
    {
        dRot = dMaxSpeed;
    }
    else if (dRot < -dMaxSpeed)
    {
        dRot = -dMaxSpeed;
    }

    // 7. Translationsanteil übernehmen
    double dTra = iter->dV;

    // 8. Geschwindigkeitsanteile überprüfen, gemäß Abbildung 34
    if (dTra * dRot > 0.0)
    {
        if (dTra + dRot > dMaxSpeed)
        {
            dTra = dMaxSpeed - dRot;
        }
        else if (dTra + dRot < -dMaxSpeed)
        {
            dTra = -dMaxSpeed - dRot;
        }
    }
    else if (dTra * dRot < 0.0)
    {
        if (dTra - dRot > dMaxSpeed)
        {
            dTra = dMaxSpeed + dRot;
        }
        else if (dTra - dRot < -dMaxSpeed)
        {
            dTra = -dMaxSpeed + dRot;
        }
    }

    // 9. Geschwindigkeiten summieren
    adWishSpeed[0] = dTra + dRot; // rechts
    adWishSpeed[1] = dTra - dRot; // links
}

double* Maneuver::GetManeuverSpeed()
{
    return adWishSpeed;
}