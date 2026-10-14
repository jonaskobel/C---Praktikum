#include "RobotControl.h"
#include <iostream>
#include <curses.h>
#include <csignal>

RobotControl* RobotControl::transferPointer;

RobotControl::RobotControl()
    : motorR(500.0, 100.0, 0.0, 0.04),
      motorL(500.0, 100.0, 0.0, 0.04)
{
    bIsActive = true;

    adActualSpeed[0] = 0.0;
    adActualSpeed[1] = 0.0;

    iMicros[0] = 1500;
    iMicros[1] = 1500;

    interface.Initialize(0.04, transferFunction);

    transferPointer = this;
}

void RobotControl::transferFunction()
{
    transferPointer->Step();
}

bool RobotControl::isActive()
{
    return bIsActive;
}

void RobotControl::Communicate()
{
    char cAntwort;
    double dRadius;
    double dSpeed;
    int iManeuver;

    std::cout << "Neues Manoever fahren? (j/n): ";
    std::cin >> cAntwort;

    if (cAntwort != 'j')
    {
        bIsActive = false;
        return;
    }

    std::cout << "Radius [m]: ";
    std::cin >> dRadius;

    std::cout << "Geschwindigkeit [m/s]: ";
    std::cin >> dSpeed;

    std::cout << "Manoever (1 = Kreis, 2 = Acht): ";
    std::cin >> iManeuver;

    if (iManeuver == 1)
    {
        maneuver.CalcCircle(dRadius, dSpeed, 0.04);
    }
    else if (iManeuver == 2)
    {
        maneuver.CalcEight(dRadius, dSpeed, 0.04);
    }
    else
    {
        return;
    }

    posEstimation.Reset();

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    printw("s = Start, b = Stop, p = Proceed, q = Abbrechen\n");
    refresh();

    // Zyklische Ausführung von Step einschalten
    sigprocmask(SIG_UNBLOCK, &interface.mask, nullptr);

    int iTaste;

    do
    {
        iTaste = getch();

        switch (iTaste)
        {
        case 's':
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
            break;
        }

        napms(10);
    }
    while (iTaste != 'q');

    // Auf den nächsten Messwert und den Stillstand warten
    do
    {
        napms(40);
    }
    while (adActualSpeed[0] != 0.0 || adActualSpeed[1] != 0.0);

    // Zyklische Ausführung von Step ausschalten
    sigprocmask(SIG_BLOCK, &interface.mask, nullptr);

    endwin();
}

void RobotControl::Step()
{
    // Istgeschwindigkeiten einmal auslesen
    double* pdInput = interface.GetInput();

    adActualSpeed[0] = pdInput[0];
    adActualSpeed[1] = pdInput[1];

    if (maneuver.isRunning())
    {
        // Position und Raumrichtung berechnen
        posEstimation.PredictPosition(
            adActualSpeed[0],
            adActualSpeed[1],
            0.04
        );

        double* pdPosition = posEstimation.GetPosition();

        // Wunschgeschwindigkeiten berechnen
        maneuver.CalcManeuverSpeed(
            pdPosition[0],
            pdPosition[1],
            pdPosition[2]
        );

        double* pdWishSpeed = maneuver.GetManeuverSpeed();

        // Stellgrößen für beide Motoren berechnen
        motorR.CalculateU(pdWishSpeed[0], adActualSpeed[0]);
        motorL.CalculateU(pdWishSpeed[1], adActualSpeed[1]);

        // Stellgrößen in Signallängen umrechnen
        iMicros[0] = (int)(1500.0 + motorR.GetU());
        iMicros[1] = (int)(1500.0 + motorL.GetU());

        // Signallängen auf [1000, 2000] begrenzen
        for (int i = 0; i < 2; i++)
        {
            if (iMicros[i] > 2000)
            {
                iMicros[i] = 2000;
            }
            else if (iMicros[i] < 1000)
            {
                iMicros[i] = 1000;
            }
        }
    }
    else
    {
        iMicros[0] = 1500;
        iMicros[1] = 1500;
    }

    interface.SetOutputs(iMicros);
}