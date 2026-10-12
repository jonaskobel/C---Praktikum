#include <ncurses.h>
#include <list>
#include <iostream>


int main() {

    std::list<char> zeichen; 

    initscr(); 
    cbreak(); 
    noecho(); 
    nodelay(stdscr, true); 

    printw("Zeichen eingeben und, mit q beenden."); 
    refresh(); 

    while (true) {
        int eingabe = getch(); 

        if (eingabe == ERR)
        {
            napms(10); 
            continue;
        }
        if (eingabe == 'q')
        {
            break; 
        }

        clear(); 

        char buchstabe = static_cast<char>(eingabe); 
        zeichen.push_back(buchstabe); 

        printw("Eingegeben: %c", buchstabe);
        refresh();
    }

    endwin(); 

    std::cout << "Zeichen rückwärts: "; 

    std::list<char>::reverse_iterator iter; 

    for(iter = zeichen.rbegin(); iter != zeichen.rend(); iter++){
        std::cout << *iter; 
    }

    std::cout << '\n'; 
}