#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

// Ein PIDController regelt einen Motor.
class PIDController
{
private:
    double dKp;
    double dKi;
    double dKd;
    double dTa;    // Zeit zwischen zwei Berechnungen, in Sekunden.
    double dEsum;  // Summe aller bisherigen Regelabweichungen.
    double deold;  // Regelabweichung aus dem vorherigen Schritt.
    double dU;     // Berechnete Stellgroesse.

public:
    PIDController(double Kp, double Ki, double Kd, double Ta);
    void CalculateU(double w, double y);
    double GetU();
};

#endif
