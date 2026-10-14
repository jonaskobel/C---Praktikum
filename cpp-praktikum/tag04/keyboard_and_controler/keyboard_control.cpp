#include "keyboard_control.h"
#include <ncurses.h>
#include <csignal>

KeyboardControl* KeyboardControl::transferPointer;

KeyboardControl::KeyboardControl()
    // Die beiden Regler werden VOR dem Code in { } erzeugt.
    // Sie brauchen jeweils vier Parameter. Diese geben wir hier hinter
    // dem Doppelpunkt an: Das nennt man Initialisierungsliste.
    : reglerRechts(500.0, 1850.0, 0.0, 0.04),
      reglerLinks(500.0, 1850.0, 0.0, 0.04)
{
    for (int i = 0; i < 2; i++)
    {
        sollgeschwindigkeit[i] = 0.0;
        istgeschwindigkeit[i] = 0.0;
        signallaengen[i] = 1500; // 1500 us bedeutet Stillstand.
    }

    // Alle 0,04 s soll die Transferfunktion und damit Step aufgerufen werden.
    // Das Timersignal ist nach Initialize zunaechst noch gesperrt.
    interface.Initialize(0.04, transferFunction);
    transferPointer = this; // this ist die Adresse dieses KeyboardControl-Objekts.
}

void KeyboardControl::transferFunction()
{
    // Die Schnittstelle kann nur eine Funktion ohne Parameter aufrufen.
    // Ueber den gespeicherten Pointer erreichen wir die Methode des Objekts.
    transferPointer->Step();
}

void KeyboardControl::Communicate()
{
    initscr();              // ncurses starten.
    cbreak();               // Tasten direkt lesen, ohne Enter.
    noecho();               // Eingaben nicht automatisch anzeigen.
    nodelay(stdscr, true);   // getch wartet nicht auf einen Tastendruck.

    bool beenden = false;
    int letzteTaste = '-';

    // Step wird ab jetzt vom Timer aufgerufen, nicht von dieser Schleife.
    sigprocmask(SIG_UNBLOCK, &interface.mask, nullptr);

    // Erste Schleife: Eingaben verarbeiten und Sollwerte einstellen.
    while (!beenden)
    {
        // Waehrend wir beide Radwerte aendern, kurz den Timer sperren.
        sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
        int taste = getch();
        if (taste != ERR) // ERR bedeutet: Es wurde keine Taste gedrueckt.
        {
            letzteTaste = taste;
            switch (taste)
            {
                case 'w':
                    sollgeschwindigkeit[0] += 0.01;
                    sollgeschwindigkeit[1] += 0.01;
                    break;
                case 's':
                    sollgeschwindigkeit[0] -= 0.01;
                    sollgeschwindigkeit[1] -= 0.01;
                    break;
                case 'a': // Linkskurve: rechts schneller, links langsamer.
                    sollgeschwindigkeit[0] += 0.005;
                    sollgeschwindigkeit[1] -= 0.005;
                    break;
                case 'd':
                    sollgeschwindigkeit[0] -= 0.005;
                    sollgeschwindigkeit[1] += 0.005;
                    break;
                case 'b': // Anhalten, aber weiter Eingaben annehmen.
                    sollgeschwindigkeit[0] = 0.0;
                    sollgeschwindigkeit[1] = 0.0;
                    break;
                case 'q': // Anhalten und die erste Schleife verlassen.
                    sollgeschwindigkeit[0] = 0.0;
                    sollgeschwindigkeit[1] = 0.0;
                    beenden = true;
                    break;
            }
        }

        // Beide Radgeschwindigkeiten einzeln auf [-0,5; 0,5] begrenzen.
        for (int i = 0; i < 2; i++)
        {
            if (sollgeschwindigkeit[i] > 0.5)
            {
                sollgeschwindigkeit[i] = 0.5;
            }
            else if (sollgeschwindigkeit[i] < -0.5)
            {
                sollgeschwindigkeit[i] = -0.5;
            }
        }

        mvprintw(0, 0, "w/s: vor/zurueck | a/d: lenken | b: Stopp | q: Ende");
        mvprintw(2, 0, "Soll rechts: % .3f m/s", sollgeschwindigkeit[0]);
        mvprintw(3, 0, "Soll links:  % .3f m/s", sollgeschwindigkeit[1]);
        mvprintw(4, 0, "Letzte Taste: %c", letzteTaste);
        refresh();

        sigprocmask(SIG_UNBLOCK, &interface.mask, nullptr);
        napms(10); // Kleine Pause, damit die Schleife nicht unnoetig schnell laeuft.
    }

    // Zweite Schleife: Erst beenden, wenn beide Raeder stehen.
    while (istgeschwindigkeit[0] != 0.0 || istgeschwindigkeit[1] != 0.0)
    {
        napms(10);
    }

    // Timer anhalten und als letzten Befehl beide Motoren auf neutral setzen.
    sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
    signallaengen[0] = 1500;
    signallaengen[1] = 1500;
    interface.SetOutputs(signallaengen);
    endwin(); // ncurses beenden.
}

void KeyboardControl::Step()
{
    // GetInput nur einmal pro Schritt aufrufen.
    // Der Pointer zeigt auf die beiden Geschwindigkeiten des Interfaces.
    double* messwerte = interface.GetInput();
    istgeschwindigkeit[0] = messwerte[0];
    istgeschwindigkeit[1] = messwerte[1];

    // Jeder Motor hat einen eigenen Regler mit seinen eigenen Messwerten.
    reglerRechts.CalculateU(sollgeschwindigkeit[0], istgeschwindigkeit[0]);
    reglerLinks.CalculateU(sollgeschwindigkeit[1], istgeschwindigkeit[1]);
    double stellwerte[2];
    stellwerte[0] = reglerRechts.GetU();
    stellwerte[1] = reglerLinks.GetU();

    for (int i = 0; i < 2; i++)
    {
        // Die PID-Stellgroesse ist ein Offset zum Neutralwert von 1500 us.
        double signal = 1500.0 + stellwerte[i];
        if (signal > 2000.0)
        {
            signal = 2000.0;
        }
        else if (signal < 1000.0)
        {
            signal = 1000.0;
        }
        signallaengen[i] = (int)signal; // Das Interface erwartet ganze Zahlen.
    }
    interface.SetOutputs(signallaengen);
}
