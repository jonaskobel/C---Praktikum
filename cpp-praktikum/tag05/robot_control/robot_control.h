#ifndef TAG05_ROBOT_CONTROL_H
#define TAG05_ROBOT_CONTROL_H

#include "InterfaceSIM.h"
#include "PIDController.h"
#include "maneuver.h"
#include "pos_estimation.h"
#include <array>
#include <csignal>

// Tag 5, Abschnitt 3.4: verbindet Benutzeroberflaeche, Manoeverplanung,
// Ortsschaetzung, zwei unabhaengige Regler und die gegebene VM-Schnittstelle.
// Im Inneren werden Geschwindigkeiten in m/s verarbeitet. Erst unmittelbar
// vor SetOutputs erfolgt die fuer InterfaceSIM notwendige Umrechnung in us.
class RobotControl {
public:
    RobotControl();
    ~RobotControl();
    RobotControl(const RobotControl&) = delete;
    RobotControl& operator=(const RobotControl&) = delete;

    void Communicate();
    void Step();
    bool isActive() const;

    // Eine statische Funktion besitzt keinen impliziten this-Parameter und
    // passt deshalb zum von Initialize erwarteten Typ void (*)(void).
    static RobotControl* transferPointer;
    static void transferFunction();

private:
    InterfaceSIM interface;
    Maneuver maneuver;
    PosEstimation posEstimation;
    PIDController motorRight;
    PIDController motorLeft;

    std::array<double, 2> actualSpeed{{0.0, 0.0}};
    std::array<double, 2> desiredSpeed{{0.0, 0.0}};
    std::array<int, 2> signalLength{{1500, 1500}};
    bool bIsActive = true;
    bool waitingForStop = false;
    unsigned int stoppedSamples = 0;
    volatile std::sig_atomic_t callbackFailed = 0;

    bool ConfigureManeuver();
    void Display(int lastKey) const;
    void SendNeutral() noexcept;
};

#endif
