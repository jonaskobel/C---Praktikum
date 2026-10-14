#ifndef MANEUVER_H
#define MANEUVER_H

#include <list>
#include <string>

// Ein Eintrag der Liste enthaelt einen Zielpunkt und seine Geschwindigkeit.
struct Coord
{
    double dX;
    double dY;
    double dZ; // Im UML heisst die Geschwindigkeit dZ; sie ist keine Hoehe.

    Coord(double x, double y, double v)
    {
        dX = x;
        dY = y;
        dZ = v;
    }
};

class Maneuver
{
private:
    std::list<Coord> coordList;
    std::list<Coord>::iterator iter; // Zeigt auf den gerade angefahrenen Punkt.
    bool bIsRunning;
    double adWishSpeed[2];          // Index 0 rechts, Index 1 links.
    double dMaxSpeed;
    double dPosDifference;

public:
    Maneuver();
    void CalcCircle(double R, double v, double T);
    void CalcEight(double R, double v, double T);
    void LogList(std::string dateiname);
    bool isRunning();
    void Start();
    void Stop();
    void Proceed();
    void CalcManeuverSpeed(double x, double y, double w);
    double* GetManeuverSpeed();
};

#endif
