#include "keyboard_control.h"
#include <ncurses.h>

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
    initscr();              // ncurses starten.
    cbreak();               // Tasten direkt lesen, ohne Enter.
    noecho();               // Eingaben nicht automatisch anzeigen.
    nodelay(stdscr, true);   // getch wartet nicht auf einen Tastendruck.

    bool beenden = false;
    int letzteTaste = '-';

    // Erste Schleife: Eingaben verarbeiten und Sollwerte einstellen.
    while (!beenden)
    {
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
        napms(10); // Kleine Pause, damit die Schleife nicht unnoetig schnell laeuft.
    }

    // Zweite Schleife: Erst beenden, wenn beide Raeder stehen.
    // In dieser Zwischenaufgabe bleiben die Istwerte noch bei null.
    while (istgeschwindigkeit[0] != 0.0 || istgeschwindigkeit[1] != 0.0)
    {
        napms(10);
    }
    endwin(); // ncurses beenden.
}

void KeyboardControl::Step()
{
    // Das Interface kommt erst in der naechsten Aufgabe hinzu.
}
