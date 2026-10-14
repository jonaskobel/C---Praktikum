#include "keyboard_control.h"

KeyboardControl::KeyboardControl()
{
    for (int i = 0; i < 2; i++)
    {
        sollgeschwindigkeit[i] = 0.0;
        istgeschwindigkeit[i] = 0.0;
    }
}

void KeyboardControl::Communicate()
{
    // In der ersten Aufgabe soll diese Methode noch leer bleiben.
    // Die Umsetzung steht im Ordner communicate_methode.
}

void KeyboardControl::Step()
{
    // In der ersten Aufgabe soll auch diese Methode noch leer bleiben.
    // Die Umsetzung steht im Ordner interfaces_and_step_methode.
}
