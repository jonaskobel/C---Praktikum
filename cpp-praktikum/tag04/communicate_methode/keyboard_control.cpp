#include "keyboard_control.h"
#include <iostream>
#include <ncurses.h>
#include <array>

KeyboardControl::KeyboardControl()
{
    sollgeschwindigkeit = {0.0, 0.0}; 
    istgeschwindigkeit = {0.0, 0.0}; 
}

void KeyboardControl::Communicate()
{
        char eingabe; 

    initscr(); 
    cbreak(); 
    noecho(); 
    nodelay(stdscr, true); 

    printw("Zeichen eingeben w, a, s, d, mit q beenden, mit b break: "); 
    refresh(); 

    while (true) {

        // getch liest einen Tastendruck, da davor nodelay(stdscr, true) gesetzt ist wartet die Funtkion nicht
        char eingabe = getch(); 

        // Taste gedrückt -> getch() liefert einen Tastencode 
        // Taste nicht gerückt -> liefert ERR
        if (eingabe == ERR)
        {
            // um CPU zu sparen warte 10 ms vor der nächsten Abfrage
            napms(10); 
            continue; 
        }

        switch (eingabe)
        {
            // erhöhe die Vorwärtsgeschwindigkeit um 0.1
            case 'w':
                sollgeschwindigkeit[0] += 0.01; // rechter motor
                sollgeschwindigkeit[1] += 0.01; // linker motor

            case 's': 
                sollgeschwindigkeit[0] -= 0.01; 
                sollgeschwindigkeit[1] -= 0.01; 

            case 'a': 
                sollgeschwindigkeit[0] += 0.005; 
                sollgeschwindigkeit[1] -= 0.005; 

            case 'd': 
                sollgeschwindigkeit[0] -= 0.005; 
                sollgeschwindigkeit[1] += 0.005; 

            case 'b': 
                sollgeschwindigkeit[0] = 0; 
                sollgeschwindigkeit[1] = 0; 

            case 'q': 
                sollgeschwindigkeit[0] = 0;
                sollgeschwindigkeit[1] = 0; 
                break; // verlässt die erste schleifen und beendet die Keyboard eingabe

            default: 
                continue;
        }

        refresh(); 
    }

    endwin(); 

    while (istgeschwindigkeit[0] != 0.0 & istgeschwindigkeit[1] != 0.0)
    {
        continue;
    }


}

void KeyboardControl::Step()
{
}