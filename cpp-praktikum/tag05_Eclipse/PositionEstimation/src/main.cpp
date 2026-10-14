#include "PositionEstimation.h"
#include <fstream>
#include <iostream>

int main()
{
	PosEstimation PosEst;

	std::fstream file;
	file.open("../PosEstimationInput.txt");

	if (!file.is_open())
	{
		std::cout << "unable to open file PosEstimationInput.txt";
		return 1;
	}

	double vrechts;
	double vlinks;
	double timestamp;

	while (file >> vrechts >> vlinks >> timestamp)
	{
		PosEst.PredictPosition(vrechts, vlinks, timestamp);
		double* position = PosEst.GetPosition();
		std::cout << "Vrechts: " << vrechts
					<< " | Vlinks: " << vlinks
					<< " | timestamp: " << timestamp
					<< " | x: " << position[0]
					<< " | y: " << position[1]
					<< " | yaw: " << position[2]
					<< '\n';
	}

	file.close();





}
