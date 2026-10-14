#include "robot_control.h"
#include "simulation_support.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>

namespace {

// Der Parameterdialog findet VOR ncurses und bei gesperrtem SIGSTEP statt.
// Ganze Zeilen verhindern, dass z.B. "0.3abc" teilweise als Zahl akzeptiert
// wird oder ein uebriges '\n' schon die naechste Eingabe beantwortet.
template<typename T>
bool ReadValue(const char* prompt, T& value)
{
    std::string line;
    while (!simulation::StopRequested()) {
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin, line)) {
            return false; // Auch EOF beendet den Dialog sauber.
        }
        std::istringstream input(line);
        std::string remainder;
        if ((input >> value) && !(input >> remainder)) {
            return true;
        }
        std::cout << "Bitte genau einen gueltigen Wert eingeben.\n";
    }
    return false;
}

} // namespace

RobotControl* RobotControl::transferPointer = nullptr;

RobotControl::RobotControl()
    // Tag 5 hat Ki = 100; die Tastaturregelung an Tag 4 benutzt Ki = 1850.
    // Der feste Ta-Wert MUSS mit dem Timer und der Ortsschaetzung uebereinstimmen.
    : motorRight(500.0, 100.0, 0.0, simulation::timestep),
      motorLeft(500.0, 100.0, 0.0, simulation::timestep)
{
    interface.Initialize(simulation::timestep, transferFunction);
    simulation::InstallStopHandlers();
    interface.SetOutputs(signalLength.data());
    // Initialize haelt SIGSTEP noch gesperrt. Erst das vollstaendig aufgebaute
    // Objekt wird zum Empfaenger des spaeter freigegebenen Timer-Callbacks.
    transferPointer = this;
}

RobotControl::~RobotControl()
{
    sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
    transferPointer = nullptr;
    SendNeutral();
}

bool RobotControl::isActive() const { return bIsActive; }

void RobotControl::SendNeutral() noexcept
{
    signalLength = {{1500, 1500}};
    // Auch im Fehlerfall versuchen wir einen Stopp zu senden. Aus einem
    // Destruktor oder Callback darf keine weitere Ausnahme herauslaufen.
    try {
        interface.SetOutputs(signalLength.data());
    } catch (...) {
    }
}

bool RobotControl::ConfigureManeuver()
{
    char answer = 'n';
    for (;;) {
        if (!ReadValue("Neues Manoever fahren? [j/n]: ", answer)) {
            bIsActive = false;
            return false;
        }
        if (answer == 'n' || answer == 'N') {
            bIsActive = false;
            return false;
        }
        if (answer == 'j' || answer == 'J') {
            break;
        }
        std::cout << "Bitte j oder n eingeben.\n";
    }

    for (;;) {
        double radius = 0.0;
        double speed = 0.0;
        char shape = 'k';
        if (!ReadValue("Radius in m (z.B. 0.3): ", radius)
            || !ReadValue("Geschwindigkeit in m/s (0 < v <= 0.5, z.B. 0.3): ", speed)
            || !ReadValue("Manoever: k = Kreis, a = Acht: ", shape)) {
            bIsActive = false;
            return false;
        }
        if (!std::isfinite(radius) || radius <= 0.0
            || !std::isfinite(speed) || speed <= 0.0 || speed > 0.5) {
            std::cout << "Radius muss positiv sein; Geschwindigkeit liegt in (0, 0.5].\n";
            continue;
        }
        try {
            if (shape == 'k' || shape == 'K') {
                maneuver.CalcCircle(radius, speed, simulation::timestep);
            } else if (shape == 'a' || shape == 'A') {
                maneuver.CalcEight(radius, speed, simulation::timestep);
            } else {
                std::cout << "Bitte k oder a als Manoever angeben.\n";
                continue;
            }
        } catch (const std::invalid_argument& error) {
            std::cout << "Parameter nicht geeignet: " << error.what() << '\n';
            continue;
        }
        return true;
    }
}

void RobotControl::Display(int lastKey) const
{
    // Aufruf nur aus einem SignalBlock: Step kann waehrenddessen weder die
    // angezeigten Daten aendern noch ncurses durch Dateioperationen unterbrechen.
    const double* position = posEstimation.GetPosition();
    erase();
    mvprintw(0, 0, "Tag 5: Kreis-/Achtmanoever | s: Start | b: Pause | p: Weiter | q: Ende");
    mvprintw(2, 0, "Status: %s", waitingForStop ? "Warte auf Stillstand"
                                    : maneuver.isRunning() ? "Faehre" : "Gestoppt / fertig");
    mvprintw(3, 0, "Position: x = % .4f m, y = % .4f m, Winkel = % .4f rad",
             position[0], position[1], position[2]);
    mvprintw(5, 0, "Soll [m/s]: rechts % .3f | links % .3f", desiredSpeed[0], desiredSpeed[1]);
    mvprintw(6, 0, "Ist  [m/s]: rechts % .3f | links % .3f", actualSpeed[0], actualSpeed[1]);
    mvprintw(7, 0, "Signal [us]: rechts %d | links %d", signalLength[0], signalLength[1]);
    mvprintw(9, 0, "Letzte Taste: %s", keyname(lastKey));
    mvprintw(10, 0, "s startet die Liste ab ihrem Anfang; p setzt nach einer Pause fort.");
    refresh();
}

void RobotControl::Communicate()
{
    // Beim erneuten Dialog wurde der vorige Lauf bereits vollstaendig gestoppt.
    sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
    if (simulation::StopRequested() || !ConfigureManeuver()) {
        bIsActive = false;
        return;
    }

    posEstimation.Reset();
    motorRight.Reset();
    motorLeft.Reset();
    desiredSpeed = {{0.0, 0.0}};
    waitingForStop = false;
    stoppedSamples = 0;
    callbackFailed = 0;

    simulation::Terminal terminal;
    // terminal bleibt bis nach dem catch lebendig: SIGSTEP wird auch bei
    // Fehlern VOR endwin gesperrt, dann stellt RAII das Terminal wieder her.
    try {
        if (sigprocmask(SIG_UNBLOCK, &interface.mask, nullptr) == -1) {
            throw std::runtime_error("Timer konnte nicht freigegeben werden.");
        }
        bool quit = false;
        int lastKey = '-';
        while (!quit) {
            {
                simulation::SignalBlock lock(interface.mask);
                if (callbackFailed) {
                    throw std::runtime_error("Fehler beim Lesen/Verarbeiten der Simulationsdaten.");
                }
                int key = getch();
                if (simulation::StopRequested()) {
                    key = 'q';
                }
                if (key != ERR) {
                    lastKey = key;
                    switch (key) {
                    case 's':
                        // Start setzt den Listeniterator zurueck, nicht das
                        // Koordinatensystem: dessen Ursprung bleibt fixiert.
                        maneuver.Start();
                        break;
                    case 'b':
                        maneuver.Stop();
                        break;
                    case 'p':
                        maneuver.Proceed();
                        break;
                    case 'q':
                        maneuver.Stop();
                        waitingForStop = true;
                        stoppedSamples = 0;
                        quit = true;
                        break;
                    default:
                        break;
                    }
                }
                Display(lastKey);
            }
            // SIGSTEP ist hier frei. Step wird vom Timer aufgerufen, NICHT
            // direkt aus der Eingabeschleife: sonst waere Ta nicht mehr 0.04 s.
            usleep(10000);
        }

        // Erst drei frische, aufeinanderfolgende Messungen nach dem Stoppbefehl
        // bestaetigen den Stillstand; initiale Nullwerte reichen nicht aus.
        for (;;) {
            bool stopped = false;
            {
                simulation::SignalBlock lock(interface.mask);
                if (callbackFailed) {
                    throw std::runtime_error("Simulationsfehler waehrend des Anhaltens.");
                }
                stopped = stoppedSamples >= 3;
                Display(lastKey);
            }
            if (stopped) {
                break;
            }
            usleep(10000);
        }
        sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
        SendNeutral();
    } catch (...) {
        sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
        maneuver.Stop();
        SendNeutral();
        bIsActive = false;
        throw;
    }
    if (simulation::StopRequested()) {
        bIsActive = false;
    }
}

void RobotControl::Step()
{
    // Genau EIN GetInput pro 40-ms-Schritt. Den Pointer nicht loeschen:
    // der Speicher gehoert InterfaceSIM. Werte gleich ins eigene Array kopieren.
    const double* input = interface.GetInput();
    for (std::size_t side = 0; side < actualSpeed.size(); ++side) {
        actualSpeed[side] = input[side];
        if (!std::isfinite(actualSpeed[side])) {
            throw std::runtime_error("Nicht-endliche Istgeschwindigkeit.");
        }
    }

    if (maneuver.isRunning()) {
        posEstimation.PredictPosition(actualSpeed[0], actualSpeed[1], simulation::timestep);
        const double* position = posEstimation.GetPosition();
        maneuver.CalcManeuverSpeed(position[0], position[1], position[2]);
        const double* wish = maneuver.GetManeuverSpeed();
        desiredSpeed = {{wish[0], wish[1]}};
    }

    // CalcManeuverSpeed kann am Listenende Stop aufrufen. Daher den Zustand
    // ERNEUT pruefen, damit noch in diesem Schritt Neutralwerte gesendet werden.
    if (maneuver.isRunning()) {
        motorRight.CalculateU(desiredSpeed[0], actualSpeed[0]);
        motorLeft.CalculateU(desiredSpeed[1], actualSpeed[1]);
        signalLength[0] = simulation::ToServoSignal(motorRight.GetU());
        signalLength[1] = simulation::ToServoSignal(motorLeft.GetU());
    } else {
        // Exakt der Stoppzweig aus Abbildung 37: keine Weiterregelung mit einem
        // alten I-Anteil. Auch Start/Proceed beginnen mit neutralem Reglerzustand.
        desiredSpeed = {{0.0, 0.0}};
        signalLength = {{1500, 1500}};
        motorRight.Reset();
        motorLeft.Reset();
    }
    interface.SetOutputs(signalLength.data());

    if (waitingForStop && simulation::IsStopped(actualSpeed)) {
        if (stoppedSamples < 3) {
            ++stoppedSamples;
        }
    } else {
        stoppedSamples = 0;
    }
}

void RobotControl::transferFunction()
{
    if (transferPointer == nullptr || transferPointer->callbackFailed) {
        return;
    }
    try {
        transferPointer->Step();
    } catch (...) {
        // Kein cout/endwin und kein Werfen ueber die Signalhandler-Grenze.
        // Communicate liest das Flag und meldet den Fehler im normalen Ablauf.
        transferPointer->callbackFailed = 1;
        transferPointer->SendNeutral();
    }
}
