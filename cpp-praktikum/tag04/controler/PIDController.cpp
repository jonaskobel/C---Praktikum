#include "PIDController.h"

PIDController::PIDController(double Kp, double Ki, double Kd, double Ta)
{
    // Die vier uebergebenen Werte in den Attributen des Objekts speichern.
    dKp = Kp;
    dKi = Ki;
    dKd = Kd;
    dTa = Ta;

    // Am Anfang gibt es noch keine frueheren Regelabweichungen.
    dEsum = 0.0;
    deold = 0.0;
    dU = 0.0;
}

void PIDController::CalculateU(double w, double y)
{
    // w ist die Sollgeschwindigkeit, y die gemessene Istgeschwindigkeit.
    double e = w - y;

    // Fuer den I-Anteil werden die Fehler der einzelnen Schritte aufaddiert.
    dEsum = dEsum + e;

    // Die drei Anteile der Formel aus dem Skript:
    double p = dKp * e;
    double i = dKi * dTa * dEsum;
    double d = dKd * (e - deold) / dTa;
    dU = p + i + d;

    // Beim naechsten Aufruf ist dieser Fehler der vorherige Fehler.
    deold = e;
}

double PIDController::GetU()
{
    return dU;
}
