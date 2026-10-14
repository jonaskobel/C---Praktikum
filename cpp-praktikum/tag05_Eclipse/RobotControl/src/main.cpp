#include "RobotControl.h"

int main()
{
    RobotControl robotControl;

    do
    {
        robotControl.Communicate();
    }
    while (robotControl.isActive());

    return 0;
}