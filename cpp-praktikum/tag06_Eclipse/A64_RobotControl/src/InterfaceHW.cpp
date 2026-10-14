#include "InterfaceHW.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <cstdint>

const std::string TA_ROBOT_MAC = "E9:4D:2B:65:7C:8B"; // Robot1: E9:4D:2B:65:7C:8B        Robot2:D2:D5:59:53:FA:72
const std::string CHAR_HANDLE = "0x0020";

// =========================================================================
// [1] Core Physical Hardware Calibration (Asymmetry Compensation)
// =========================================================================
// [Purpose]: The real STM32 robot exhibits severe hardware asymmetry. 
// Applying the exact same PWM to both motors causes the robot to drive in circles.
// [Why these values?]: The left motor is heavily overpowered compared to the right. 
// By keeping the left scale at 1.0 and restricting the right scale to a calibrated 
// fraction (e.g., 0.12), we force the hardware to align with pure mathematical 
// models at the HAL (Hardware Abstraction Layer).
#define MOTOR_LEFT_SCALE   1.0
#define MOTOR_RIGHT_SCALE  0.12 

// =========================================================================
// [2] Static Friction / Deadzone Compensation
// =========================================================================
// [Purpose]: Real DC motors have static friction (stiction). If the calculated
// PWM value is too low (e.g., 3 or 5), the motor will whine but not rotate.
// [Why 15?]: Empirical testing showed that a PWM command of at least 15 is 
// required to break the static friction. This acts as a "deadzone lifter", 
// ensuring that any non-zero logical command results in actual physical movement.
#define MIN_PWM 15
#define MAX_SAFE_SPEED 400

static void (*g_pTimerFunction)(void) = nullptr;

void TimerHandler(int signum) { if (g_pTimerFunction) g_pTimerFunction(); }

InterfaceHW::InterfaceHW() : m_pGattPipe(nullptr) {
    m_sRobotMac = TA_ROBOT_MAC;
    m_sHandle = CHAR_HANDLE;
    m_dCurrentSpeed[0] = 0.0;
    m_dCurrentSpeed[1] = 0.0;
    
    // [Architecture Shift]: Initiate Bluetooth Low Energy (BLE) connection via 
    // Linux 'gatttool' running in a unidirectional pipe. This replaces the old 
    // local GPIO pin control.
    std::string cmd = "gatttool -i hci0 -t random -b " + m_sRobotMac + " -I > /dev/null 2>&1";
    m_pGattPipe = popen(cmd.c_str(), "w");
    if (m_pGattPipe) {
        fprintf(m_pGattPipe, "connect\n");
        fflush(m_pGattPipe);
        sleep(2); // Block and wait for the BLE handshake to complete
    }
}

InterfaceHW::~InterfaceHW() {
    if (m_pGattPipe) {
        SendBluetoothCommand(0, 0); // Failsafe: Ensure robot stops before disconnecting
        fprintf(m_pGattPipe, "exit\n");
        pclose(m_pGattPipe);
    }
}

void InterfaceHW::Initialize(double dTimeStep) {}

void InterfaceHW::SetOutputs(int* iMicros) {
    if (iMicros == nullptr) return;

    // Convert standard servo microsecond signals (1500 = neutral) to delta ranges
    int raw_R = iMicros[0] - 1500;
    int raw_L = iMicros[1] - 1500;

    // STEP 1: Calculate "Logical Speed"
    // Normalize the input (-500 to +500) into our high-resolution scale (MAX_SAFE_SPEED).
    int16_t logical_R = (int16_t)((raw_R / 500.0) * MAX_SAFE_SPEED);
    int16_t logical_L = (int16_t)((raw_L / 500.0) * MAX_SAFE_SPEED);

    // STEP 2: Calculate "Physical Speed"
    // Apply the asymmetry calibration scales. The software logic remains perfectly
    // symmetrical, while the hardware mismatch is absorbed here.
    int16_t phys_R = (int16_t)(logical_R * MOTOR_RIGHT_SCALE);
    int16_t phys_L = (int16_t)(logical_L * MOTOR_LEFT_SCALE);

    // STEP 3: Apply Deadzone Lifter
    // If the algorithm demands movement (logical > 5), ensure the physical output 
    // is strong enough to break static friction (>= MIN_PWM).
    if (abs(logical_R) > 5) { 
        if (phys_R > 0 && phys_R < MIN_PWM) phys_R = MIN_PWM;
        if (phys_R < 0 && phys_R > -MIN_PWM) phys_R = -MIN_PWM;
    }
    
    if (abs(logical_L) > 5) {
        if (phys_L > 0 && phys_L < MIN_PWM) phys_L = MIN_PWM;
        if (phys_L < 0 && phys_L > -MIN_PWM) phys_L = -MIN_PWM;
    }

    // Hardware Limit Protection to prevent Bluetooth packet overflow or motor burnout
    phys_R = std::max((int16_t)-MAX_SAFE_SPEED, std::min((int16_t)MAX_SAFE_SPEED, phys_R));
    phys_L = std::max((int16_t)-MAX_SAFE_SPEED, std::min((int16_t)MAX_SAFE_SPEED, phys_L));

    // STEP 4: Dispatch the final physical command via BLE
    SendBluetoothCommand(phys_L, phys_R);

    // STEP 5: Generate Mock Feedback for Upper Layers
    // [Purpose]: Since we abandoned the noisy and unreliable Hall encoders, 
    // returning 0.0 here would cause upper-layer state machines (like PosEstimation) 
    // to stall. We feed back a mathematically simulated velocity to keep the 
    // algorithm loops running smoothly.
    double max_vel = 0.5; 
    double feedback_factor = 0.8; 
    m_dCurrentSpeed[0] = ((double)logical_R / MAX_SAFE_SPEED) * max_vel * feedback_factor;
    m_dCurrentSpeed[1] = ((double)logical_L / MAX_SAFE_SPEED) * max_vel * feedback_factor;
}

// =========================================================================
// [3] Anti-Bloat Pipe Filter & BLE Dispatcher
// =========================================================================
void InterfaceHW::SendBluetoothCommand(int left, int right) {
    if (!m_pGattPipe) return;

    // Static memory to track the previously dispatched command
    static int last_left = -999;
    static int last_right = -999;
    static int same_count = 0;

    // [Core Anti-Blocking Logic]: 
    // Linux pipes can easily buffer hundreds of redundant Bluetooth commands 
    // if the upper layer sends data too fast (e.g., 25Hz), causing extreme 
    // execution lag (the robot executes stale commands seconds later).
    // Here we strictly deduplicate identical consecutive commands.
    if (left == last_left && right == last_right) {
        same_count++;
        // Allow exactly one duplicate command to pass every 5 cycles (~0.5s).
        // This acts as a "Heartbeat" signal to prevent the BLE receiver 
        // on the STM32 from entering sleep mode or triggering a timeout halt.
        if (same_count < 5) return; 
    }
    
    // Update state for the next cycle
    same_count = 0;
    last_left = left;
    last_right = right;

    char hex_val[20];
    
    // Encode the integer speeds into the specific 4-byte Little-Endian HEX 
    // payload required by the STM32 firmware's GATT characteristic.
    sprintf(hex_val, "%02x%02x%02x%02x",
            (uint8_t)(left & 0xFF), (uint8_t)((left >> 8) & 0xFF),
            (uint8_t)(right & 0xFF), (uint8_t)((right >> 8) & 0xFF));
            
    // Dispatch the string command into the gatttool pipe and force flush 
    // the buffer to ensure zero-latency transmission.
    fprintf(m_pGattPipe, "char-write-cmd %s %s\n", m_sHandle.c_str(), hex_val);
    fflush(m_pGattPipe);
}

double* InterfaceHW::GetInput() { 
    return m_dCurrentSpeed; 
}

void InterfaceHW::SetTimerfunction(int iTimer, double dTimeStep, void (*pFunction)(void)) {
    // Intentionally left blank. 
    // The asynchronous hardware timer architecture has been deprecated in favor 
    // of synchronous, thread-safe sleep delays (usleep) in the main control loop.
}