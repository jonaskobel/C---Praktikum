#include "robot_control.h"
#include <exception>
#include <iostream>

int main()
{
    try {
        // Nur ein Steuerungsobjekt pro Prozess: InterfaceSIM verwendet einen
        // statischen Timer sowie einen einzelnen statischen Funktionspointer.
        RobotControl control;
        do {
            control.Communicate();
        } while (control.isActive());
    } catch (const std::exception& error) {
        // Communicate hat SIGSTEP gesperrt und ncurses bereits beendet.
        std::cerr << "RobotControl: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
