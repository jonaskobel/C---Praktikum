#ifndef KEYBOARD_CONTROL_H
#define KEYBOARD_CONTROL_H


class KeyboardControl
{
private:
    // Index 0 gehoert zum rechten Motor, Index 1 zum linken Motor.
    double sollgeschwindigkeit[2];
    double istgeschwindigkeit[2];

public:
    KeyboardControl();
    void Communicate();
    void Step();
};

#endif
