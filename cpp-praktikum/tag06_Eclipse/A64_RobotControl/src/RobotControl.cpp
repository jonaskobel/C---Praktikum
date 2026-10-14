#include "RobotControl.h"
#include <unistd.h> 

RobotControl::RobotControl() {
	bIsActive = true; 
	cEingabe = -1;
	iMicros[0] = 1500; iMicros[1] = 1500;
	interface.Initialize(0.1);
}

RobotControl::~RobotControl() {}
bool RobotControl::isActive() { return bIsActive; }

void RobotControl::Communicate() {
	char man = -1;
	std::cout << "Maneuver starten? (1: Ja): "; std::cin >> man;
	if (man == '1') bIsActive = true; else bIsActive = false;

	if (bIsActive == true) {
		double dRadius = 0, dSpeed = 0;
		std::cout << "Radius (m): "; std::cin >> dRadius;
		std::cout << "Geschwindigkeit (m/s): "; std::cin >> dSpeed;
		std::cout << "Manoever (1: Kreis, 2: Acht): "; std::cin >> man;

		if (man == '1') maneuver.SetManeuver(1, dRadius, dSpeed);
		if (man == '2') maneuver.SetManeuver(2, dRadius, dSpeed);

		initscr(); nodelay(stdscr, TRUE); noecho();

		do {
			cEingabe = getch(); 
			if ((cEingabe != -1 && cEingabe != 'q')) {
				switch (cEingabe) {
				case '1': maneuver.Start(); break;
				case '2': maneuver.Stop(); break;
				case '3': maneuver.Proceed(); break;
				}
			}
            
			clear();
			printw("Running... \n1: Start | 2: Pause | 3: Continue | q: Quit\n\n");
            
			if (maneuver.isRunning()) {
				printw(">>> Status: RUNNING <<<\n");
			} else {
				printw(">>> Status: PAUSED / STOPPED <<<\n");
			}
            
			printw("Time: %.1f sec / %.1f sec\n", maneuver.GetTimer(), maneuver.GetTargetTime());
			printw("L PWM: %d | R PWM: %d\n", iMicros[1], iMicros[0]);
			refresh();

			// ==============================================================================
			//  LÜCKE 5: Software-Loop & Drosselung
			// ==============================================================================
			// AUFGABE: Da wir keinen Hardware-Timer mehr verwenden (um Bluetooth zu entlasten),
			// müssen wir die 'Step()'-Methode hier in der Endlosschleife manuell aufrufen.
			// Drosseln Sie die Schleife anschließend mit einem 'usleep', um eine Zykluszeit 
			// von 0.1 Sekunden (100.000 Mikrosekunden) zu erreichen.
			// TODO: Rufen Sie Step() auf und fügen Sie das usleep ein:
			
			/* DEIN CODE HIER (2 Zeilen) */
			Step();
			usleep(100000);
			
			
		} while (cEingabe != 'q');

		maneuver.Stop(); 
		Step(); // Letzter Aufruf, um den Stopp-Befehl an die Hardware zu senden
		endwin();
	}
}

void RobotControl::Step() {
	// ==============================================================================
	// LÜCKE 6: Zeitschritt der Maneuver-Klasse
	// ==============================================================================
	// AUFGABE: Treiben Sie die Zustandsmaschine (State Machine) in Maneuver voran.
	// Übergeben Sie den Zeitschritt (dt = 0.1 Sekunden), der unserem usleep entspricht.
	// TODO: Rufen Sie maneuver.Step(...) auf:
	
	/* DEIN CODE HIER (1 Zeile) */
	maneuver.Step(0.1);
	

	if (maneuver.isRunning() == false) {
		iMicros[0] = 1500; iMicros[1] = 1500; 
	} else {
		// ==============================================================================
		// LÜCKE 7: Open-Loop PWM Mapping (Die radikalste Änderung für Tag 6)
		// ==============================================================================
		// FRAGE: An Tag 5 haben wir hier den PID-Regler aufgerufen. Warum jetzt nicht mehr?
		// ANTWORT: Ein geschlossener Regelkreis ist über Bluetooth zu stark verzögert.
		// AUFGABE: Holen Sie sich die berechneten Geschwindigkeiten (GetManeuverSpeed).
		// Mappen Sie diese DIREKT in PWM-Signale (Faktor 1000, Nulllage 1500).
		// TODO: Berechnen Sie iMicros[0] und iMicros[1]:
		
		/* DEIN CODE HIER (~3 Zeilen) */
		double* dVSoll = maneuver.GetManeuverSpeed();
		iMicros[0] = 1500 + 1000.0 * (int)(dVSoll[0]);
		iMicros[1] = 1500 + 1000.0 * (int)(dVSoll[1]);
		
		
		

		// Hardware-Schutz (Clamping)
		if(iMicros[0]>2000) iMicros[0]=2000; if(iMicros[0]<1000) iMicros[0]=1000;
		if(iMicros[1]>2000) iMicros[1]=2000; if(iMicros[1]<1000) iMicros[1]=1000;
	}
	
	interface.SetOutputs(iMicros);
}
