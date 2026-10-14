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

    initscr(); 
    cbreak(); 
    noecho(); 
    nodelay(stdscr, true); 

    printw("w/s: vor/zurueck | a/d: lenken | b: Stopp | q: Ende\n"); 
    refresh(); 

    bool beenden = false; 
    int letzte_taste = '-'; 

    while (!beenden) {

        // getch liest einen Tastendruck, da davor nodelay(stdscr, true) gesetzt ist wartet die Funtkion nicht
        int eingabe = getch(); 

        // Taste gedrückt -> getch() liefert einen Tastencode 
        // Taste nicht gerückt -> liefert ERR 
        if (eingabe != ERR)
        {
            letzte_taste = eingabe; 

            switch (eingabe)
            {
                // erhöhe die Vorwärtsgeschwindigkeit um 0.01
                case 'w':
                    sollgeschwindigkeit[0] += 0.01; // rechter motor
                    sollgeschwindigkeit[1] += 0.01; // linker motor
                    break; 

                case 's': 
                    sollgeschwindigkeit[0] -= 0.01; 
                    sollgeschwindigkeit[1] -= 0.01; 
                    break; 

                case 'a': 
                    sollgeschwindigkeit[0] += 0.005; 
                    sollgeschwindigkeit[1] -= 0.005; 
                    break; 

                case 'd': 
                    sollgeschwindigkeit[0] -= 0.005; 
                    sollgeschwindigkeit[1] += 0.005; 
                    break;

                case 'b': 
                    sollgeschwindigkeit = {0.0, 0.0}; 
                    break;

                case 'q': 
                    sollgeschwindigkeit = {0.0, 0.0}; 
                    beenden = true; // beednet die erste schleife
                    break; 

                default: 
                    break;
            }
        }


        for (double& v : sollgeschwindigkeit)
        {
            if (v > 0.5) 
            {
                v = 0.5; 
            }
            else if (v < -0.5)
            {
                v = -0.5; 
            }
            
        }

         // Auch ohne Tastendruck regelmaessig aufrufen
        Step();

        mvprintw(2, 0, "Soll rechts: % .3f m/s",
                 sollgeschwindigkeit[0]);

        mvprintw(3, 0, "Soll links:  % .3f m/s",
                 sollgeschwindigkeit[1]);

        mvprintw(4, 0, "Letzte Taste: %s",
                 keyname(letzte_taste));

        // Reste einer vorherigen, laengeren Tastenanzeige loeschen
        clrtoeol();
        refresh();
        napms(10);
    }


    while (istgeschwindigkeit[0] != 0.0 || istgeschwindigkeit[1] != 0.0)
    {
        Step(); // rufe die Methode stes auf um die auf die soll Geschwindigkeit zu korrigieren
        napms(10);  // warte 10 ms 
    }

    endwin(); 


}

void KeyboardControl::Step()
{
}