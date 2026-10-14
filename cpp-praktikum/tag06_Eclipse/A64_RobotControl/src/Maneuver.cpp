#include "Maneuver.h"
#include <cmath>

#define TURN_COEFF 0.09            
#define TIME_CAL_LEFT 1.1  // Zeit-Korrektur für den ersten Kreis (Links)
#define TIME_CAL_RIGHT 1.4 // Zeit-Korrektur für den zweiten Kreis (Rechts)

Maneuver::Maneuver() {
	bIsRunning = false;
	iManeuverType = 1;
	dTimer = 0.0;
	dTargetTimeLoop1 = 0.0;
	dTargetTimeLoop2 = 0.0;
	adWishSpeed[0] = 0.0; adWishSpeed[1] = 0.0;
}

Maneuver::~Maneuver() {}

void Maneuver::SetManeuver(int type, double radius, double speed) {
	iManeuverType = type;
	dTimer = 0.0;
	bIsRunning = false;

	// ==============================================================================
	// LÜCKE 1: Berechnung der Zielzeit (Open-Loop Kinematik)
	// ==============================================================================
	// FRAGE: An Tag 5 haben wir eine Liste von (X,Y)-Koordinaten berechnet. Warum 
	// berechnen wir jetzt stattdessen eine ZIELZEIT (Target Time)?
	// ANTWORT: Weil die Positionsabschätzung durch Radschlupf versagt. Wir steuern
	// den Roboter nun blind nach Zeit.
	// AUFGABE: Berechnen Sie die theoretische Zeit für einen Kreis: T = (2 * PI * Radius) / Geschwindigkeit.
	// Multiplizieren Sie das Ergebnis anschließend mit den Konstanten TIME_CAL_LEFT bzw. TIME_CAL_RIGHT.
	// TODO: Berechnen Sie dTargetTimeLoop1 und dTargetTimeLoop2:
	
	/* DEIN CODE HIER (2 Zeilen) */
	dTargetTimeLoop1 = (2.0 * M_PI * radius / speed) * TIME_CAL_LEFT;
	dTargetTimeLoop2 = (2.0 * M_PI * radius / speed) * TIME_CAL_RIGHT;
	
	

	double diff = TURN_COEFF / radius; 
	
	// ==============================================================================
	//  LÜCKE 2: Anti-Spin Lock (Schutz vor Rückwärtsdrehen)
	// ==============================================================================
	// AUFGABE: Bei sehr kleinen Radien wird 'diff' größer als 'speed'. Das würde dazu 
	// führen, dass das kurveninnere Rad rückwärts dreht (Drehen auf der Stelle).
	// Begrenzen Sie 'diff' so, dass (speed - diff) immer mindestens 0.02 beträgt!
	// TODO: Fügen Sie die if-Bedingung ein:
	
	/* DEIN CODE HIER (~3 Zeilen) */
	if (speed - diff < 0.002) {
		diff = speed -0.02;
	}
	
	

	// ====== Kreis 1 (Links) ======
	dSpeedL_Loop1 = speed - diff; 
	dSpeedR_Loop1 = speed + diff; 

	// ====== Kreis 2 (Rechts für die Acht) ======
	// Der rechte Motor ist physisch extrem schwach. Um eine Rechtskurve zu schaffen,
	// geben wir dem linken Rad einen massiven 'Overdrive' (Faktor 1.6).
	dSpeedL_Loop2 = speed + (diff * 1.6); 
    
	dSpeedR_Loop2 = speed - diff; 
	// Verhindern, dass das schwache rechte Rad durch die Motorreibung (Deadzone) stehen bleibt.
	if (dSpeedR_Loop2 < 0.12) {
		dSpeedR_Loop2 = 0.12; 
	}
}

void Maneuver::Start() { bIsRunning = true; dTimer = 0.0; }
void Maneuver::Stop() { bIsRunning = false; adWishSpeed[0] = 0.0; adWishSpeed[1] = 0.0; }
void Maneuver::Proceed() { bIsRunning = true; } 
bool Maneuver::isRunning() { return bIsRunning; }

void Maneuver::Step(double dt) {
	if (!bIsRunning) {
		adWishSpeed[0] = 0.0; adWishSpeed[1] = 0.0;
		return;
	}

	// ==============================================================================
	// LÜCKE 3: Timer hochzählen
	// ==============================================================================
	// AUFGABE: Dies ist das Herzstück der State Machine. Zählen Sie die Zeitvariable hoch!
	// TODO: Addieren Sie den Zeitschritt 'dt' auf 'dTimer':
	
	/* DEIN CODE HIER (1 Zeile) */
	dTimer += dt;
	

	if (iManeuverType == 1) { 
		// ====== Manöver: Einzelner Kreis ======
		if (dTimer <= dTargetTimeLoop1) {
			adWishSpeed[0] = dSpeedR_Loop1;
			adWishSpeed[1] = dSpeedL_Loop1;
		} else {
			Stop(); 
		}
	} 
	else if (iManeuverType == 2) { 
		// ==============================================================================
		//  LÜCKE 4: State Machine für die Acht (Figure-8)
		// ==============================================================================
		// AUFGABE: Programmieren Sie den Ablauf für die Acht. 
		// 1. Wenn dTimer <= dTargetTimeLoop1: Fahre Kreis 1 (Loop1)
		// 2. Wenn dTimer <= (dTargetTimeLoop1 + dTargetTimeLoop2): Fahre Kreis 2 (Loop2)
		// 3. Sonst: Stopp!
		// TODO: Vervollständigen Sie die if-else-Struktur:
		
		/* DEIN CODE HIER (~8 Zeilen) */
		if (dTimer <= dTargetTimeLoop1) {
			adWishSpeed[0] = dSpeedR_Loop1;
			adWishSpeed[1] = dSpeedL_Loop1;
		} else if (dTimer <= dTargetTimeLoop1 + dTargetTimeLoop2) {
			adWishSpeed[0] = dSpeedR_Loop2;
			adWishSpeed[1] = dSpeedL_Loop2;
		} else {
			Stop();
		}
		
		
		
		
		
	}
}

double* Maneuver::GetManeuverSpeed() { return adWishSpeed; }
