#include "polymorphie.h"

int main()
{
    // Aufgabe Polymorphie, Skript Seite 73, Listing 68 auf Seite 75.
    // Die Aufgabe verlangt, die Ausgaben durch Ueberlegen zu bestimmen.
    // Die Antworten stehen jeweils vor der zugehoerigen Anweisung.

    // a) Zeile 30: Kompiliert. a ist ein Objekt der Klasse A.
    // f ruft h auf, und h ruft i auf.
    // Ausgabe:
    // A.f
    // A.h
    // A.i
    A a = A();
    a.f();

    // a) Zeile 31: Kompiliert. b ist ein Objekt der Klasse B.
    // B::g ruft B::f auf. B::f ruft zuerst A::f und danach A::h auf.
    // Weil A::f selbst schon h aufruft, erscheinen h und i zweimal.
    // Ausgabe:
    // B.g
    // B.f
    // A.f
    // A.h
    // A.i
    // A.h
    // A.i
    B b = B();
    b.g();

    // a) Zeile 32: Kompiliert. c ist ein Objekt der Klasse C.
    // C::g ruft das geerbte B::f auf. Dabei bleibt h die Version von A,
    // weil h nicht virtuell ist. i ist virtuell und verwendet deshalb C::i.
    // C.h erscheint also NICHT in dieser Ausgabe.
    // Ausgabe:
    // C.g
    // B.f
    // A.f
    // A.h
    // C.i
    // A.h
    // C.i
    C c = C();
    c.g();

    // b) Zeile 34: Kompiliert NICHT.
    // Aus einem A-Objekt kann nicht automatisch ein B-Objekt erzeugt werden.
    // Ein A ist nicht unbedingt ein B, auch wenn jedes B ein A ist.
    // Deshalb bleiben diese Anweisungen auskommentiert:
    // B b0 = A();
    // b0.g();
    // Ohne das Auskommentieren wuerde das gesamte Programm nicht
    // kompilieren und es gaebe keine Programmausgabe.

    // b) Zeile 35: Kompiliert. Vom B-Objekt wird nur der A-Anteil kopiert.
    // Das nennt man Object Slicing. a1 ist danach ein echtes A-Objekt.
    // Ausgabe:
    // A.f
    // A.h
    // A.i
    A a1 = B();
    a1.f();

    // b) Zeile 36: Kompiliert. Auch vom vorhandenen b wird nur der A-Anteil
    // kopiert. a2 ist ein A-Objekt; das urspruengliche b bleibt ein B-Objekt.
    // Ausgabe:
    // A.f
    // A.h
    // A.i
    A a2 = b;
    a2.f();

    // b) Zeile 37: Kompiliert. a3 speichert nur die Adresse von b.
    // Es wird kein Objekt kopiert. Hinter dem A-Pointer steckt weiterhin
    // ein B-Objekt. Weil f virtuell ist, wird B::f aufgerufen.
    // Ausgabe:
    // B.f
    // A.f
    // A.h
    // A.i
    // A.h
    // A.i
    A* a3 = &b;
    a3->f();

    // Antwort auf die letzte Frage aus b):
    // Pointer sind hier besser als das Kopieren in ein Elternklassenobjekt.
    // Das urspruengliche Objekt und sein Typ bleiben erhalten. Virtuelle
    // Methoden koennen dadurch weiterhin die Version der Kindklasse aufrufen.

    return 0;
}
