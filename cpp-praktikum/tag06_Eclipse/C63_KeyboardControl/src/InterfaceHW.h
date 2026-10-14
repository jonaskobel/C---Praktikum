#ifndef INTERFACE_HW_H_
#define INTERFACE_HW_H_

#include <string>
#include <cstdio> 

// =========================================================================
// Physical Robot Parameters
// =========================================================================
#define WHEEL_DIAMETER  0.040
#define TRACK_WIDTH     0.090

// =========================================================================
// Global Safety & Resolution Constraint
// =========================================================================
// [Why 400?]: We are dealing with extreme hardware asymmetry where the right
// motor requires a very small scaling factor (e.g., 0.12 or 0.047). If our 
// maximum speed limit was too small (e.g., 10 or 100), multiplying it by 0.12 
// would result in severe precision loss (integer truncation). Setting a higher 
// resolution range (400) preserves the granularity of the calculation before 
// sending it to the hardware.
#define MAX_SAFE_SPEED  400 

class InterfaceHW {
public:
    InterfaceHW();
    virtual ~InterfaceHW();

    void Initialize(double dTimeStep);
    double* GetInput();
    void SetOutputs(int* iMicros);
    
    // Deprecated in the current event-driven architecture, 
    // but kept for compatibility with the upper-layer interface definition.
    void SetTimerfunction(int iTimer, double dTimeStep, void (*pFunction)(void));

private:
    std::string m_sRobotMac;
    std::string m_sHandle;
    double m_dCurrentSpeed[2];
    FILE* m_pGattPipe;

    void SendBluetoothCommand(int left_speed, int right_speed);
};

#endif /* INTERFACE_HW_H_ */