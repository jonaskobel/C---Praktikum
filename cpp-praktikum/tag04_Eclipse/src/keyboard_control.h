#ifndef KEYBOARD_CONTROL_K
#define KEYBOARD_CONTROL_K
#include "InterfaceSIM.h"
#include "PIDController.h"

#include <array>


class KeyboardControl
{
private:

	InterfaceSIM interfaceSIM;

    std::array<double, 2> sollgeschwindigkeit; // std::array<double, 2> sollgescwindigkeit{}; würde auch gehen. Stellt mehrere methoden zur verfügung wie size(), at()...
    std::array<double, 2> istgeschwindigkeit; 
    int iMicros[2];

    PIDController reglerRechts;
    PIDController reglerLinks;

public: 
    KeyboardControl(); 

    void Communicate(); 
    void Step(); 

    static KeyboardControl* transferPointer;
    static void transferFunction();
}; 



#endif
