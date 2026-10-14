#ifndef POS_ESTIMATION_H
#define POS_ESTIMATION_H

class PosEstimation
{
private:
    double x[3];        // x[0]: x-Koordinate, x[1]: y-Koordinate, x[2]: Winkel.
    double dVelAverage; // Mittlere Geschwindigkeit des vorherigen Schritts.

public:
    PosEstimation();
    void Reset();
    void PredictPosition(double rechts, double links, double T);
    double* GetPosition();
};

#endif
