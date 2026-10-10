#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date
{
private:
    int m_day, m_month, m_year; 

public: 
    Date(int day, int month, int year);

    // neuer Standardkonstruktor 
    Date(); 
    bool isEqual(Date dd); 
    int getDay(); 
    void setDay(int day); 
    int getMonth(); 
    int getYear(); 
    int compare(Date dd); 
}; 

#endif
