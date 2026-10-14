#include "KeyboardControl.h"
#include <unistd.h> // for usleep

// Der Pointer für eine mögliche parallele Ausführung (wird an Tag 6 nicht genutzt)
KeyboardControl* KeyboardControl::transferPointer;

KeyboardControl::KeyboardControl() 
{
	dVSoll[0] = 0.0; dVSoll[1] = 0.0;
	dVIst[0] = 0.0; dVIst[1] = 0.0;
	iMicros[0] = 1500; iMicros[1] = 1500;
	cEingabe = -1;

	interface.Initialize(0.1);
	
	// ==============================================================================
	// LÜCKE 1: Hardware-Timer (Architektur-Verständnis)
	// ==============================================================================
	// FRAGE: An Tag 4/5 haben wir hier 'interface.SetTimerfunction(...)' aufgerufen.
	// Warum dürfen wir den Timer hier für die reale Hardware (Bluetooth) NICHT mehr nutzen?
	// (Tipp: Denken Sie an "Buffer Bloat" und Bluetooth-Latenzen).
	// LÖSUNG: (Kein Code notwendig. Verstehen Sie, warum die Zeile fehlt!)
	
	//Lsg:
	// ein Timer der alle 25 ms starr ein Befehl abfeuert führt zu einem Datenstau (buffer Bloat) wodurch Befehle erst Sekunden später
	// am Roboter ankommen (Lags)

	transferPointer = this;
}

KeyboardControl::~KeyboardControl() {}

void KeyboardControl::Communicate()
{
	initscr();
	nodelay(stdscr, TRUE);
	noecho();

	printw("KeyboardControl (Hardware Mode - Event Driven)\n");
	printw("w/s/a/d/b/q to control\n");

	do
	{
		cEingabe = getch();
		
		// Wir reagieren nur, wenn tatsächlich eine Taste gedrückt wurde
		if (cEingabe != -1 && cEingabe != 'q') 
		{
			clear();
			switch(cEingabe)
			{
			case 'w': dVSoll[0] += 0.05; dVSoll[1] += 0.05; break;
			case 's': dVSoll[0] -= 0.05; dVSoll[1] -= 0.05; break;
			case 'a': dVSoll[0] += 0.05; dVSoll[1] -= 0.05; break;
			case 'd': dVSoll[0] -= 0.05; dVSoll[1] += 0.05; break;
			case 'b': dVSoll[0] = 0.0; dVSoll[1] = 0.0; break;
			}

			// Geschwindigkeitsbegrenzung [-0.5, 0.5]
			if(dVSoll[0] > 0.5) dVSoll[0] = 0.5;
			if(dVSoll[0] < -0.5) dVSoll[0] = -0.5;
			if(dVSoll[1] > 0.5) dVSoll[1] = 0.5;
			if(dVSoll[1] < -0.5) dVSoll[1] = -0.5;

			printw("Target L: %f R: %f\n", dVSoll[1], dVSoll[0]);
			printw("Key: %c\n", cEingabe);
            
			// ==============================================================================
			// LÜCKE 2: Event-Driven Sending (Ereignisgesteuertes Senden)
			// ==============================================================================
			// AUFGABE: Rufen Sie hier die Methode auf, die das Signal an den Roboter sendet.
			// WARUM HIER?: Wir senden den Befehl NUR DANN, wenn der Benutzer die 
			// Geschwindigkeit wirklich geändert hat. Das verhindert, dass redundante 
			// Befehle den Bluetooth-Kanal verstopfen.
			// TODO: Fügen Sie den Methodenaufruf ein:
			
			/* DEIN CODE HIER (1 Zeile) */
			Step();
			
		}
        
		// ==============================================================================
		// LÜCKE 3: CPU-Auslastung reduzieren
		// ==============================================================================
		// AUFGABE: Fügen Sie eine Schlafpause von 50 Millisekunden (50000 Mikrosekunden) ein.
		// Das verhindert, dass unsere "while"-Schleife 100% der CPU-Leistung frisst.
		// TODO: Fügen Sie den Sleep-Befehl ein:
		
		/* DEIN CODE HIER (1 Zeile) */
		usleep(50000);
		

	} while(cEingabe != 'q');

	endwin();
}

void KeyboardControl::Step()
{
	// Mock-Feedback: Die Ist-Geschwindigkeit entspricht direkt der Soll-Geschwindigkeit
	dVIst[0] = dVSoll[0]; 
	dVIst[1] = dVSoll[1];

	// ==============================================================================
	// LÜCKE 4: Open-Loop PWM Mapping (Direkte Umrechnung ohne Regler)
	// ==============================================================================
	// AUFGABE: Rechnen Sie die gewünschte Soll-Geschwindigkeit (dVSoll) in m/s 
	// direkt in ein PWM-Signal (iMicros) um. Wir nutzen absichtlich KEINEN PID-Regler mehr!
	// HINWEIS: Die neutrale Nulllage ist 1500. Der Faktor für die Umrechnung ist 1000.
	// TODO: Berechnen Sie iMicros[0] (rechter Motor) und iMicros[1] (linker Motor):
	
	/* DEIN CODE HIER (2 Zeilen) */
	iMicros[0] = 1500 + 1000.0 * (int)(dVSoll[0]);
	iMicros[1] = 1500 + 1000.0 * (int)(dVSoll[1]);
	
	

	// Hardware-Schutzgrenzen (Clamp), um Motoren und Bluetooth-Pakete zu schützen
	if(iMicros[0]>2000) iMicros[0]=2000; if(iMicros[0]<1000) iMicros[0]=1000;
	if(iMicros[1]>2000) iMicros[1]=2000; if(iMicros[1]<1000) iMicros[1]=1000;

	// Befehl an die Hardware-Schnittstelle senden
	interface.SetOutputs(iMicros);
}

void KeyboardControl::transferFunction() {
	transferPointer -> Step();
}
