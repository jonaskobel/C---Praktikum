#ifndef VEHICLE_H
#define VEHICLE_H
#include <iostream>


class vehicle
{

public: 
    enum color{blue=0, red, green, white, black};  // muss zuerst definiert werden um späte verwendet werden zu können

private:
    color m_color; // enum color definiert nur das objekt  color m_color definiert die Farbe des Fahrzeuges
    double price; 
    int year_of_construction; 
    int idn; 

    static int next_id; // als statisches Objekt der Klasse definiert 

public: 
    vehicle(color c, double price, int year_of_construction); // jetzt Konstruktor defnieren 

    // static methods can be used without initializing the class
    //static bool isOldtimer(vehicle v);  es wird nur eine kopie von vehicle übergeben 
    static bool isOldtimer(const vehicle &v); //Referenz aus das orginal aber nicht veränderbar
    //static bool isOldtimer(const vehicle* v); // würde auch gehen, dann zugriff statt über v. über v->

    std::string getColor() const; 
    double getPrice() const; 
    int getYearOfConstruction() const; 
    int getId() const; 
}; 


#endif