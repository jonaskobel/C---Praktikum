#ifndef SEPR_SIMULATION_SUPPORT_H
#define SEPR_SIMULATION_SUPPORT_H

#include <array>
#include <cmath>
#include <csignal>
#include <stdexcept>
#include <ncurses.h>

// Kleine gemeinsame Hilfen fuer die SIMULATIONSprogramme an Tag 4 und Tag 5.
// Die eigentlichen Lernaufgaben (Step, PID, Ortsschaetzung, Manoever) bleiben
// in ihren Klassen. Alle Motorarrays verwenden Index 0 = rechts, 1 = links.
namespace simulation {

inline constexpr double timestep = 0.04; // 40 ms = 25 Simulationsschritte/s

// Ein Signal darf Communicate nicht mitten beim Aendern beider Sollwerte oder
// einer Koordinatenliste unterbrechen. SignalBlock sperrt SIGSTEP fuer einen
// kurzen Bereich und stellt beim Verlassen die VORHERIGE Maske wieder her.
// So funktionieren auch verschachtelte Bereiche und Ausnahmen korrekt.
// Das ist die signalbasierte Praktikumsloesung, kein std::thread-Mutex.
class SignalBlock {
public:
    explicit SignalBlock(const sigset_t& mask)
    {
        if (sigprocmask(SIG_BLOCK, &mask, &oldMask) == -1) {
            throw std::runtime_error("Timersignal konnte nicht gesperrt werden.");
        }
    }

    ~SignalBlock() { sigprocmask(SIG_SETMASK, &oldMask, nullptr); }
    SignalBlock(const SignalBlock&) = delete;
    SignalBlock& operator=(const SignalBlock&) = delete;

private:
    sigset_t oldMask{};
};

// RAII: ncurses wird auch dann beendet, wenn nach seinem Start eine Ausnahme
// auftritt. Fehlermeldungen in main erscheinen dadurch wieder im normalen
// Terminal. Nur Communicate benutzt ncurses; Step schreibt nie auf den Bildschirm.
class Terminal {
public:
    Terminal()
    {
        if (initscr() == nullptr) {
            throw std::runtime_error("ncurses konnte nicht gestartet werden.");
        }
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE); // getch liefert ERR, wenn keine Taste vorliegt
        curs_set(0);
    }

    ~Terminal() { endwin(); }
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;
};

// PID-Ausgang u ist bereits ein Offset in Mikrosekunden. Fuer die ungeregelte
// Variante wird hier stattdessen 1000 * v_soll uebergeben. Die Begrenzung
// erfolgt VOR dem Umwandeln in int, damit auch grosse Stellwerte sicher sind.
inline int ToServoSignal(double offset)
{
    if (!std::isfinite(offset)) {
        throw std::runtime_error("Ungueltiger Motorstellwert.");
    }
    double signal = 1500.0 + offset;
    if (signal > 2000.0) {
        signal = 2000.0;
    } else if (signal < 1000.0) {
        signal = 1000.0;
    }
    return static_cast<int>(std::lround(signal));
}

inline bool IsStopped(const std::array<double, 2>& speed)
{
    // Statt == 0: 1 mm/s Toleranz fuer numerisch abklingende Simulationswerte.
    // Der Aufrufer wartet zusaetzlich mehrere NEUE Messungen ab. Nicht-endliche
    // Werte duerfen insbesondere niemals als Stillstand interpretiert werden.
    constexpr double tolerance = 0.001;
    return std::isfinite(speed[0]) && std::isfinite(speed[1])
        && std::abs(speed[0]) <= tolerance && std::abs(speed[1]) <= tolerance;
}

inline volatile std::sig_atomic_t stopRequested = 0;

// Im Ctrl+C-/SIGTERM-Handler nur ein atomar lesbares Flag setzen: kein cout,
// kein ncurses, kein Dateizugriff. Communicate beendet danach wie bei 'q' die
// Fahrt und wartet auf Stillstand, waehrend der 40-ms-Timer weiterarbeitet.
inline void RequestStop(int) { stopRequested = 1; }
inline bool StopRequested() { return stopRequested != 0; }
inline void ClearStopRequest() { stopRequested = 0; }

inline void InstallStopHandlers()
{
    // Vor der Installation loeschen, damit ein unmittelbar danach empfangenes
    // Signal nicht versehentlich wieder zurueckgesetzt wird.
    ClearStopRequest();
    struct sigaction action{};
    action.sa_handler = RequestStop;
    sigemptyset(&action.sa_mask);
    // Initialize installiert selbst einen SIGINT-Handler. Deshalb diese
    // Funktion erst NACH interface.Initialize aufrufen.
    if (sigaction(SIGINT, &action, nullptr) == -1
        || sigaction(SIGTERM, &action, nullptr) == -1) {
        throw std::runtime_error("Abbruchbehandlung konnte nicht eingerichtet werden.");
    }
}

} // namespace simulation

#endif
