#ifndef POSITION_ESTIMATION_P
#define POSITION_ESTIMATION_P

#include <array>

class PosEstimation
{
private:
	std::array<double, 3> x;
	double dVelAverage;

public:
	PosEstimation();

	void Reset();
	void PredictPosition(double dSpeedR, double dSpeedL, double dTimestep);
	double* GetPosition(); // liefert nur die Adresse des Ergebnisarrays x3 zurück, keine kopie

};

#endif
