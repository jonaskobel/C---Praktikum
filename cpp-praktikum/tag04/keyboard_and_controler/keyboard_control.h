#ifndef KEYBOARD_CONTROL_H
#define KEYBOARD_CONTROL_H

#include "InterfaceSIM.h"
#include "PIDController.h"

class KeyboardControl
{
private:
    InterfaceSIM interface;
    PIDController reglerRechts;
    PIDController reglerLinks;
    // Index 0 gehoert zum rechten Motor, Index 1 zum linken Motor.
    double sollgeschwindigkeit[2];
    double istgeschwindigkeit[2];
    int signallaengen[2]; // Signallaengen in Mikrosekunden.

public:
    KeyboardControl();
    void Communicate();
    void Step();

    // Fuer den periodischen Aufruf von Step, wie in Abschnitt 2.7 beschrieben.
    static KeyboardControl* transferPointer;
    static void transferFunction();
};

#endif
