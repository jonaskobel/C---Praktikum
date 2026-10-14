#include "maneuver.h"

int main()
{
    Maneuver maneuver;

    maneuver.CalcCircle(2.0, 0.3, 0.04);
    maneuver.LogList("LogFileCircle.txt");

    maneuver.CalcEight(5.0, 1.0, 10.0);
    maneuver.LogList("LogFileEight.txt");

    return 0;
}