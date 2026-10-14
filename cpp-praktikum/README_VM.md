# Tag 4 und Tag 5 auf der VM

Die Aufgaben aus Kapitel 2 und 3 des `SEPR_Skript2026.pdf` sind als C++-Quelltexte
umgesetzt. Die Kommentare erklären Rechenwege, Einheiten, Datenübergaben und
Programmablauf. Die vorhandenen VM-Module und Übungsdaten werden vorausgesetzt.
Die Programme wurden hier **nicht kompiliert oder ausgeführt**. Inbetriebnahme und
Vergleich mit der Simulation erfolgen wie gewünscht erst auf der VM.

## Kopieren und bauen

Den gesamten Ordner `cpp-praktikum` auf die Linux-VM kopieren. `tag04`, `tag05`,
`common`, `CMakeLists.txt` und `build_vm.sh` gehören zusammen; gemeinsame Klassen
werden von mehreren Aufgaben verwendet. Vorhandene macOS-Buildordner müssen
nicht mitkopiert werden.

Im Terminal in den kopierten Ordner wechseln und ausführen:

```bash
bash build_vm.sh
```

Das Skript verwendet die auf der VM vorhandenen Werkzeuge CMake und g++ sowie
die Bibliotheken `ncurses`, `jsoncpp` und `rt`. Es installiert nichts und erzeugt
die Programme in einem neuen Ordner `build-vm`. Es startet keine Simulation.
Alle Programme verwenden C++17. Alternativ können nur die beiden fertigen
Steuerungen direkt gebaut werden (für die späteren Dateibeispiele den
vollständigen Build mit `bash build_vm.sh` verwenden):

```bash
cmake -S . -B build-vm -DSEPR_BUILD_SIMULATION=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-vm --target tag04_keyboard_and_controler tag05_robot_control
```

## Aufgabenübersicht

Die Seitenzahlen beziehen sich auf die tatsächlichen PDF-Seiten; das
Inhaltsverzeichnis des Skripts ist teilweise gegenüber diesen verschoben.

| Aufgabe | Quellordner | Programm in `build-vm` |
| --- | --- | --- |
| Tag 4: Klasse mit zwei Geschwindigkeitsarrays, S. 101 | `tag04/keyboard_control` | `tag04_keyboard_control` |
| Tag 4: Communicate mit Tastatur und Anzeige, S. 101–104 | `tag04/communicate_methode` | `tag04_communicate_methode` |
| Tag 4: Interface, Step und Timer-Parallelisierung, S. 104–108 | `tag04/interfaces_and_step_methode` | `tag04_interfaces_and_step_methode` |
| Tag 4: Regler einschließlich Dateibeispiel, S. 109–111 | `tag04/controler` | `tag04_pid_controller` |
| Tag 4: Tastatursteuerung mit zwei Reglern, S. 112–113 | `tag04/keyboard_and_controler` | `tag04_keyboard_and_controler` |
| Tag 5: Positionsschätzung einschließlich Dateivergleich, S. 114–117 | `tag05/positionsschaetzung` | `tag05_positionsschaetzung` |
| Tag 5: Kreis-/Achtliste und Logdateien, S. 118–120 | `tag05/maneuver`, `tag05/koordinatenliste` | `tag05_koordinatenliste` |
| Tag 5: Start/Stop/Proceed, Sollgeschwindigkeiten, S. 120–125 | `tag05/maneuver` | Bestandteil von `tag05_robot_control` |
| Tag 5: RobotControl, Communicate, Step und main, S. 125–130 | `tag05/robot_control` | `tag05_robot_control` |

Die ersten beiden Tag-4-Projekte sind die separaten Lernstände der jeweiligen
Aufgabe. Im ersten sind die Methoden ausdrücklich noch leer, wie dort verlangt;
das zweite demonstriert die Eingabe ohne Simulationsanschluss. Die beiden
vollständigen fahrfähigen Tag-4-Varianten sind die Interface- und die Reglervariante.

## Tag 4: Tastatursteuerung

Zuerst die vorhandene Simulation `MoonRover.x86_64` über ihre Verknüpfung auf der
VM starten. Optional den **Evaluation Mode** für die gefahrene Route aktivieren.
Anschließend im VM-Terminal genau eines der folgenden Programme starten:

```bash
# Ungeregelte Variante: v_soll wird direkt auf die Signallänge abgebildet.
./build-vm/tag04_interfaces_and_step_methode

# Vollständige Variante mit je einem PI-Regler für rechts und links.
./build-vm/tag04_keyboard_and_controler
```

| Taste | Wirkung |
| --- | --- |
| `w` / `s` | Beide Sollgeschwindigkeiten um +0,01 / -0,01 m/s ändern |
| `a` | Rechts +0,005, links -0,005 m/s |
| `d` | Rechts -0,005, links +0,005 m/s |
| `b` | Beide Sollgeschwindigkeiten auf null setzen |
| `q` | Anhalten, auf Stillstand warten und Programm beenden |

Die Rad-Sollgeschwindigkeiten bleiben jeweils in [-0,5; +0,5] m/s. Der Timer
ruft `Step()` alle 0,04 s auf. `Communicate()` ruft `Step()` nicht zusätzlich auf.
Strg+C wird bei aktiver Steuerung ebenfalls als geregeltes Beenden behandelt.

## Tag 4: Regler mit Originaldaten

Die beiden auf der VM bzw. dem Übungslaufwerk vorhandenen Dateien
`PIDControllerInput.txt` und `PIDControllerVgl.txt` in den aktuellen Arbeitsordner
legen. Danach:

```bash
./build-vm/tag04_pid_controller
```

Alternativ beide Dateipfade als Argumente übergeben:

```bash
./build-vm/tag04_pid_controller /pfad/PIDControllerInput.txt /pfad/PIDControllerVgl.txt
```

Das Programm liest die zehn Soll-/Istwertpaare, berechnet die Stellgrößen mit
`Kp=1000`, `Ki=10`, `Kd=0.1`, `Ta=0.5` und zeigt die Originalvergleichsdatei
darunter an. Der Sichtvergleich ist in dieser Aufgabe ausdrücklich vorgesehen.
Es werden keine eigenen Referenzwerte anstelle der Originaldatei verwendet.

## Tag 5: Positionsschätzung

Die Originaldateien `PosEstimationInput.txt` und `PosEstimationVgl.txt` in den
aktuellen Arbeitsordner legen oder beide Pfade als Argumente übergeben:

```bash
./build-vm/tag05_positionsschaetzung
```

Die Eingabespalten sind **rechts, links, Zeitschritt**. Die Ausgabespalten sind
**x, y, Winkel**. Zusätzlich erfolgt ein numerischer Vergleich mit der Referenz;
die absolute Toleranz beträgt standardmäßig `1e-5` und kann als drittes Argument
angepasst werden, falls die Originaldatei gröber gerundet ist. Exitcode 0 bedeutet
Übereinstimmung, 2 eine Abweichung und 1 einen Datei-/Eingabefehler.

Die Spurbreite der Simulation ist `B=0.23 m`. Die Berechnungsreihenfolge entspricht
dem Skript: zunächst x/y mit **vorheriger** mittlerer Geschwindigkeit und
**vorherigem** Winkel, dann den neuen Winkel berechnen und zuletzt die neue
mittlere Geschwindigkeit speichern. Der Winkel liegt anschließend in ]-π; π].

## Tag 5: Koordinatenlisten

```bash
./build-vm/tag05_koordinatenliste
```

Dadurch entstehen im aktuellen Arbeitsordner:

- `LogFileCircle.txt`: Kreis mit `R=2.0`, `v=0.3`, `T=0.04` (1046 Punkte).
- `LogFileEight.txt`: Acht mit `R=5.0`, `v=1.0`, `T=10.0` (vier Punkte).

Die ungewöhnlichen Werte der Acht entsprechen exakt der Dateiaufgabe im Skript.
Sie sind nicht die Fahrparameter der Robotersteuerung. Beide Dateien enthalten
je Zeile nur x und y, getrennt durch einen Tabulator. Sie können direkt in das
vorhandene VM-Projekt `Kapitel5_Koordinatenliste_Test` kopiert und dort geprüft
werden. Die Logdateien werden erst beim Ausführen dieses Aufgabenprogramms
erzeugt; die C++-Programme wurden hier nicht gestartet.

## Tag 5: Autonome Manöversteuerung

Die Simulation zuerst starten, dann:

```bash
./build-vm/tag05_robot_control
```

Im Dialog `j` wählen, Radius und Geschwindigkeit eingeben und anschließend
`k` für Kreis oder `a` für Acht wählen. Die Beispielwerte aus dem Skript sind
**Radius 0.3 m und Geschwindigkeit 0.3 m/s**. Dezimalzahlen mit Punkt eingeben.

Danach übernimmt ncurses die Kommunikation:

| Taste | Wirkung |
| --- | --- |
| `s` | Manöver ab dem ersten Listenelement starten |
| `b` | Manöver unterbrechen; aktuellen Listenpunkt behalten |
| `p` | Unterbrochenes Manöver fortsetzen |
| `q` | Manöver anhalten und nach Stillstand zum Parameterdialog zurückkehren |

Im anschließenden Parameterdialog beendet `n` das Programm. Strg+C hält an und
beendet das gesamte Programm. Ein fertiges Manöver stoppt automatisch; `p`
setzt eine vollständig abgefahrene Liste nicht neu in Gang. `s` beginnt erneut
am Listenanfang. Der beim Dialog festgelegte Koordinatenursprung bleibt dabei
erhalten; nur ein neu konfiguriertes Manöver setzt die Positionsschätzung zurück.

Die Wegpunkt-Toleranz beträgt 2 cm. Die Winkelabweichung wird auf ]-π; π]
begrenzt und mit Faktor 2 in einen Rotationsanteil umgerechnet. Die Translation
wird nötigenfalls reduziert, sodass die Rotation erhalten bleibt und jedes Rad
höchstens 0,5 m/s als Sollwert erhält.

## Gemeinsam genutzte Vorgaben

| Stelle | Kp | Ki | Kd | Abtastzeit |
| --- | ---: | ---: | ---: | ---: |
| Regler-Dateiaufgabe | 1000 | 10 | 0.1 | 0.5 s |
| Tastatursteuerung Tag 4 | 500 | 1850 | 0 | 0.04 s |
| Manöversteuerung Tag 5 | 500 | 100 | 0 | 0.04 s |

Motorarrays enthalten überall rechts an Index 0 und links an Index 1. Die
Simulationssignale werden auf [1000; 2000] µs begrenzt; 1500 µs bedeutet Stopp.
Die PID-Stellgröße wird direkt als Offset auf 1500 µs addiert. Die Skalierung
`1500 + 1000*v` gehört ausschließlich zur ungeregelten Steuerung.

`common/simulation_support.h` enthält die kommentierten Hilfen für ncurses,
Signalsperren, Signalbegrenzung und Stoppsignale. Das Ende einer Fahrt erfordert
drei aufeinanderfolgende Messwertabfragen nach dem Stoppbefehl mit höchstens
0,001 m/s Betrag pro Rad; so wird ein
numerisch abklingender Simulationswert nicht endlos mit exakt null verglichen.

Das vorgegebene `InterfaceSIM.h/.cpp` liegt weiterhin unverändert unter
`tag04/interfaces_and_step_methode`. Seine tatsächlich verwendeten JSON-Pfade
sind `/home/pi/Schreibtisch/actual_value.json` und
`/home/pi/Schreibtisch/target_value.json`. Diese müssen zu den Dateien der
vorhandenen VM-Simulation passen; falls die VM eine angepasste Version des
Lehrstuhlmoduls bereitstellt, deren Dateien an dieser Stelle verwenden und neu
bauen. Immer nur **ein** Steuerungsprogramm gleichzeitig starten, weil die
Programme dieselben Simulationsdateien beschreiben.

Die Timer-Anbindung folgt dem signalbasierten Verfahren aus dem Skript.
Zugriffe der Benutzeroberfläche auf gemeinsame Daten sind gegen SIGSTEP
geschützt. Die vorgegebene Schnittstelle selbst erledigt JSON-Dateizugriffe im
Signalhandler; sie ist damit keine allgemeine Vorlage für eine signal- oder
echtzeitsichere Produktionssteuerung.

Bei pausiertem Manöver gibt Tag 5 gemäß Abbildung 37 Neutralwerte aus und
berechnet keine neue Position. Für `Proceed()` bleibt die zuletzt geschätzte
Position erhalten. Die Positionsschätzung hat damit dieselbe Vereinfachung
bezüglich des Ausrollens wie die Aufgabenstellung.

## Falls die Projekte in Eclipse eingebunden werden

Für jedes ausführbare Programm nur seine eigene `main.cpp` und seine benötigten
Implementierungen verwenden. Mehrere `main.cpp` dürfen nicht in dasselbe
Programm gelangen. Der CMake-Build übernimmt diese Zuordnung bereits.

Für die fertige Tag-5-Steuerung werden `robot_control.cpp`,
`pos_estimation.cpp`, `maneuver.cpp`, `PIDController.cpp` und `InterfaceSIM.cpp`
mit `tag05/robot_control/main.cpp` übersetzt. Als Include-Verzeichnisse dienen
`common`, `tag04/controler`, `tag04/interfaces_and_step_methode`,
`tag05/positionsschaetzung` und `tag05/maneuver`. Für die fertige Tag-4-Steuerung
sind es deren eigene `keyboard_control.cpp`/`main.cpp`, `PIDController.cpp`
und `InterfaceSIM.cpp` sowie die ersten drei genannten Include-Verzeichnisse.
Bei beiden im Linker `ncurses`, `rt`, `jsoncpp` ergänzen und C++17 einschalten.
