#ifndef ROBOTCONTROL_H_
#define ROBOTCONTROL_H_

#include <iostream>
#include <ncurses.h>

#include "InterfaceHW.h"
#include "Maneuver.h"


class RobotControl {
public:
	RobotControl();
	virtual ~RobotControl();
	bool isActive();
	void Communicate();
	void Step();

private:
	InterfaceHW interface;
	Maneuver maneuver;
	bool bIsActive;
	char cEingabe;
	int iMicros[2];
};

#endif /* ROBOTCONTROL_H_ */
