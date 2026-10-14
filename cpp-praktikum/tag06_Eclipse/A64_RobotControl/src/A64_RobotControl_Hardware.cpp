#include <iostream>
#include "RobotControl.h"

int main() {

	std::cout << "Willkommen bei RobotControl!" << std::endl;

	RobotControl robotcontrol;

	do {
		robotcontrol.Communicate();
	} while (robotcontrol.isActive() == true);

	std::cout << "Programmende!" << std::endl;

	return 0;
}
