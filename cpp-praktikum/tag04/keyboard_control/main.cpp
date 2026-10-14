#include "keyboard_control.h"

int main()
{
    KeyboardControl control;
    control.Communicate();
    control.Step();
    return 0;
}
