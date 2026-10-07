# Erklärung zu SEPR_Skript2026, Seiten 35–38

Auf diesen Seiten geht es darum, wie du Speicher für Variablen und Arrays bereitstellst, über Pointer darauf zugreifst und den Speicher wieder freigibst. Anschließend wendest du diese Kenntnisse beim Zählen von Zufallszahlen und beim Sortieren an.

Quelle: [SEPR_Skript2026.pdf](../../../SEPR_Skript2026.pdf), Seiten 35–38. Die folgenden Erklärungen ergänzen und korrigieren einige vereinfachte Aussagen im Skript.

## Seite 35: Referenzen und Speicherverwaltung

Die erste Aufgabe ist die bereits besprochene `swap`-Funktion:

```cpp
void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}
```

Durch die Referenzen verändert die Funktion die ursprünglichen Variablen. Diese Funktion kannst du später beim Bubblesort wiederverwenden.

Danach führt das Skript zwei Arten der Speicherverwaltung ein:

| Eigenschaft | Lokale Variablen, typischerweise auf dem Stack | Dynamischer Speicher, auf dem Heap |
|---|---|---|
| Beispiel | `int zahlen[10];` | `int* zahlen = new int[n];` |
| Größe des eingebauten Arrays | Muss hier zur Übersetzungszeit feststehen | Kann zur Laufzeit bestimmt werden |
| Lebensdauer | Bis zum Verlassen des zugehörigen Blocks | Bis zur Freigabe |
| Freigabe | Automatisch | Bei dieser Verwendung von `new[]` durch `delete[]` |

**Korrektur zum Skript:** Normale lokale Variablen haben automatische, nicht statische Speicherdauer. Echte statische Variablen leben während der gesamten Programmausführung. Auch darfst du auf lokale Variablen in beliebiger Reihenfolge zugreifen; das LIFO-Prinzip beschreibt die Stapelorganisation, keine vorgeschriebene Lesereihenfolge.

## Seite 36: `new` reserviert Speicher, `delete` gibt ihn frei

Bei einer einzelnen Zahl:

```cpp
int* p = new int{5};  // Eine int-Variable anlegen und mit 5 initialisieren

std::cout << *p;      // Wert lesen: 5

delete p;            // Speicher freigeben
p = nullptr;         // Dieser Pointer zeigt jetzt auf nichts
```

Bei einem Array:

```cpp
int n = 10;
int* zahlen = new int[n];  // Platz für 10 int-Werte

zahlen[0] = 42;
zahlen[1] = 17;

delete[] zahlen;           // Das ganze Array freigeben
zahlen = nullptr;
```

Dabei bedeutet:

- `new int[n]`: Reserviere Platz für `n` Ganzzahlen. Deren Werte sind zunächst nicht initialisiert.
- `int* zahlen`: Speichere die Adresse des ersten Elements.
- `zahlen[i]`: Greife auf das Element am Index `i` zu.
- `delete[] zahlen`: Gib den reservierten Array-Speicher wieder frei.

**Die Indizes gehen von `0` bis `n - 1`.** Bei zehn Elementen ist `zahlen[9]` das letzte; `zahlen[10]` liegt bereits außerhalb.

`int* p` allein reserviert noch keinen zusätzlichen Speicher für einen Zahlenwert. Erst `new int` legt hier das Objekt an, auf das `p` zeigen soll. Der Pointer selbst ist ebenfalls eine Variable und benötigt eigenen Speicher.

Die Zuordnung muss stimmen:

```cpp
new int       // gehört zu delete
new int[n]    // gehört zu delete[]
```

**Korrektur zum Skript:** Bei einem falschen `delete` wird nicht einfach nur der „Kopf“ des Arrays freigegeben. `delete` auf ein mit `new[]` erzeugtes Array führt zu undefiniertem Verhalten. Nach der Freigabe darfst du dessen Elemente nicht mehr verwenden.

## Seite 37: Aufgabe „Speicher und Arrays“

### Teil a: Warum ist das große lokale Array problematisch?

Im Beispiel steht:

```cpp
int iStack[100000000];
```

Das sind 100 Millionen Ganzzahlen. Bei üblichen vier Byte pro `int` benötigt das ungefähr **400 MB**. Für den normalerweise deutlich kleineren Stack ist das zu viel. Wenn das Array tatsächlich angelegt wird, droht ein Stacküberlauf und damit ein Programmabsturz. Ein Compiler kann das im Beispiel unbenutzte Array allerdings auch wegoptimieren.

Der vorgesehene Lösungsansatz ist dynamischer Speicher:

```cpp
int* zahlen = new int[100000000];

// Array verwenden ...

delete[] zahlen;
```

Auch der Heap ist begrenzt, aber du vermeidest damit die kleine Stackgrenze.

### Teil b: Zufallszahlen erzeugen und zählen

Für diesen Teil brauchst du **100.000 Elemente**. Gehe in diesen Schritten vor:

1. Zufallsgenerator einmal am Anfang initialisieren.
2. Array dynamisch anlegen.
3. Alle Elemente mit Zufallszahlen füllen.
4. Durch das Array gehen und passende Zahlen zählen.
5. Den Zähler ausgeben.
6. Den Speicher freigeben.

Die folgenden Codeabschnitte sind Bausteine für dein Programm. Die Includes stehen am Dateianfang, die übrigen Anweisungen kannst du in dieser Reihenfolge in `main` einsetzen.

#### Benötigte Bibliotheken

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
```

#### Zufallsgenerator initialisieren

```cpp
std::srand(static_cast<unsigned int>(std::time(nullptr)));
```

`std::time(nullptr)` liefert einen zeitabhängigen Startwert. `static_cast<unsigned int>` wandelt ihn in den von `srand` erwarteten Typ um. Initialisiere den Generator **einmal vor den Schleifen**. Bei gleichem Startwert entsteht dieselbe Zahlenfolge; insbesondere können kurz aufeinanderfolgende Programmstarts denselben zeitbasierten Startwert erhalten.

#### Array anlegen

```cpp
const int anzahl = 100000;
int* zahlen = new int[anzahl];
```

#### Array füllen

```cpp
for (int i = 0; i < anzahl; ++i)
{
    zahlen[i] = std::rand() % 101;
}
```

**Warum `% 101`?** Der Rest bei einer Division durch 101 liegt zwischen `0` und `100`, jeweils einschließlich. Allgemein liefert `rand() % n + m` für positives `n` Werte von `m` bis `m + n - 1`.

#### Durch 13 teilbare Zahlen zählen

```cpp
int zaehler = 0;

for (int i = 0; i < anzahl; ++i)
{
    if (zahlen[i] % 13 == 0)
    {
        ++zaehler;
    }
}
```

`%` berechnet den Divisionsrest. Beispielsweise:

```cpp
26 % 13  // 0: ohne Rest teilbar
27 % 13  // 1: nicht ohne Rest teilbar
```

Auch **0 zählt mit**, denn `0 % 13 == 0`.

#### Ergebnis ausgeben und Speicher freigeben

```cpp
std::cout << "Durch 13 teilbar: " << zaehler << '\n';
delete[] zahlen;
```

Teste zunächst mit zehn Elementen und gib deren Werte aus. So kannst du den Zähler von Hand überprüfen, bevor du auf 100.000 erhöhst.

## Seite 38: Aufgabe „Bubblesort“

Bubblesort sortiert, indem es immer **zwei benachbarte Elemente vergleicht und bei falscher Reihenfolge tauscht**.

Beispiel für aufsteigende Sortierung:

```text
Start:                    4  2  3  1

4 und 2 vergleichen:      2  4  3  1
4 und 3 vergleichen:      2  3  4  1
4 und 1 vergleichen:      2  3  1  4
```

Nach diesem ersten Durchlauf steht die größte Zahl ganz rechts. Dann wiederholst du das Verfahren für den verbleibenden Bereich:

```text
Nach zweitem Durchlauf:   2  1  3  4
Nach drittem Durchlauf:   1  2  3  4
```

### Teil a: Sortierfunktion entwickeln

Diese Signatur ist geeignet:

```cpp
void bubble_sort(int* zahlen, int anzahl)
```

`zahlen` zeigt auf das erste Array-Element. `anzahl` musst du zusätzlich übergeben, weil der Pointer die Arraygröße nicht enthält.

Baue die Funktion aus zwei Schleifen auf:

- Die äußere Schleife zählt die Durchläufe, von `i = 0` solange `i < anzahl - 1`.
- Die innere Schleife vergleicht benachbarte Elemente.
- Wenn das linke Element größer ist, rufst du deine `swap`-Funktion auf.

Der entscheidende Teil lautet:

```cpp
if (zahlen[j] > zahlen[j + 1])
{
    swap(zahlen[j], zahlen[j + 1]);
}
```

Hier passt deine Referenzfunktion genau: `zahlen[j]` und `zahlen[j + 1]` sind die beiden tatsächlichen Array-Elemente, deren Werte getauscht werden. Definiere deine `swap`-Funktion vor `bubble_sort` oder deklariere sie dort bereits.

Bei einem äußeren Schleifenzähler `i`, der bei `0` beginnt, kann die innere Schleife so laufen:

```cpp
for (int j = 0; j < anzahl - 1 - i; ++j)
```

Das `-1` verhindert einen Zugriff hinter das Array durch `j + 1`. Das `-i` spart die Elemente rechts aus, die bereits an ihrer endgültigen Position stehen.

### Teil b: In `main` testen

Gehe ähnlich wie auf Seite 37 vor:

1. Mit `new int[32]` ein Array anlegen.
2. Zufallszahlen eintragen; den Generator vorher einmal initialisieren.
3. Das unsortierte Array ausgeben.
4. `bubble_sort(zahlen, 32);` aufrufen.
5. Das sortierte Array ausgeben.
6. Mit `delete[] zahlen;` den Speicher freigeben.

Teste die Sortierfunktion zuerst mit einer kleinen bekannten Folge wie `4, 2, 3, 1`. Prüfe danach auch bereits sortierte Werte und doppelte Zahlen.

### Teil c: Effizienz beurteilen

Bubblesort benötigt im schlechtesten Fall ungefähr `n² / 2` Vergleiche, also **O(n²)**. Bei doppelter Elementanzahl steigt der Aufwand ungefähr auf das Vierfache. Zusätzlicher Speicher ist dagegen nur in konstanter Menge nötig, etwa für die Zwischenvariable beim Tauschen: **O(1)**.

Als Verbesserung kannst du abbrechen, wenn in einem vollständigen Durchlauf kein Tausch nötig war. Mit dieser Verbesserung benötigt ein bereits sortiertes Array nur einen Durchlauf, also **O(n)** Zeit.

## Verständnisfragen auf Seite 38

### 1. Was ist der Unterschied zwischen dynamischer und statischer Speicherreservierung?

Das Skript stellt hier lokale automatische Variablen dem dynamischen Speicher gegenüber. Lokale automatische Variablen werden beim Verlassen ihres Blocks aufgeräumt. Mit `new` reservierter Speicher muss bei diesen Übungen passend mit `delete` oder `delete[]` freigegeben werden. Seine Lebensdauer ist nicht automatisch an den Block gebunden.

Echte statische Variablen sind davon zu unterscheiden: Sie leben während der gesamten Programmausführung.

### 2. Wann benutzt man dynamische Speicherreservierung?

Wenn die benötigte Größe erst zur Laufzeit bekannt ist, Daten den aktuellen Block überleben sollen oder große Arrays den Stack überfordern würden.

### 3. Welche Gefahren gehen von Pointern aus?

- Zugriff über ungültige oder nicht initialisierte Adressen.
- Dereferenzieren von `nullptr`.
- Zugriff nach der Freigabe des Objekts.
- Zugriff außerhalb eines Arrays.
- Doppelte oder falsche Freigabe.
- Vergessene Freigabe dynamisch reservierten Speichers: ein Speicherleck.

### 4. Welche Vorteile haben Referenzen gegenüber Pointern?

Referenzen bieten eine einfachere Schreibweise und eine feste Bindung an ein Objekt. Sie müssen initialisiert werden und haben keinen vorgesehenen Nullzustand. Sie können aber ebenfalls ungültig werden, wenn das referenzierte Objekt nicht mehr existiert.
