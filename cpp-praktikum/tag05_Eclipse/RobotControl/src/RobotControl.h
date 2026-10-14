#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include "InterfaceSIM.h"
#include "maneuver.h"
#include "PIDController.h"
#include "PositionEstimation.h"

class RobotControl
{
private:
    InterfaceSIM interface;
    Maneuver maneuver;
    PosEstimation posEstimation;

    PIDController motorR;
    PIDController motorL;

    bool bIsActive;
    double adActualSpeed[2];
    int iMicros[2];

public:
    RobotControl();

    static RobotControl* transferPointer;
    static void transferFunction();

    bool isActive();
    void Communicate();
    void Step();
};

#endif