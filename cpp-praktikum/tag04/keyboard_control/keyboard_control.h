#ifndef KEYBOARD_CONTROL_K
#define KEYBOARD_CONTROL_K
#include <array>

class KeyboardControl
{
private:
    std::array<double, 2> sollgeschwindigkeit; // std::array<double, 2> sollgescwindigkeit{}; würde auch gehen. Stellt mehrere methoden zur verfügung wie size(), at()...
    std::array<double, 2> istgeschwindigkeit; 

public: 
    KeyboardControl(); 
    void Communicate(); 
    void Step(); 
}; 


#endif