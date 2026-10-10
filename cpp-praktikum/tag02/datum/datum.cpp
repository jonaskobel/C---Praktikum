#include "datum.h"
#include <iostream>
#include <cstdlib>

Date::Date(int day, int month, int year) // überladener Konstruktor 
{
    m_day = day;
    m_month = month; 
    m_year = year; 
}

// zweiter Konstruktor 
Date::Date(){
    int days_per_month[12] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    }; 

    m_year = 1970 + std::rand() % 61; 
    m_month = 1 + std::rand() %12; 
    m_day = std::rand() % days_per_month[m_month-1]; 
}

int Date::compare(Date dd)
{
    if (dd.m_year < m_year)
    {
        return 1; 
    }
    if (dd.m_year > m_year)
    {
        return -1; 
    }
    // same year
    if (dd.m_month < m_month)
    {
        return 1; 
    }
    if (dd.m_month > m_month)
    {
        return -1; 
    }
    // same month 
    if (dd.m_day < m_day)
    {
        return 1; 
    }
    if (dd.m_day > m_day)
    {
        return -1; 
    }
    return 0; 

}

int Date::getDay()
{
    return m_day; 
}

void Date::setDay(int day)
{
    m_day = day; 
}

int Date::getMonth()
{
    return m_month; 
}

int Date::getYear()
{
    return m_year; 
}

bool Date::isEqual(Date dd)
{
    if(
        m_day==dd.m_day &&
        m_month==dd.m_month &&
        m_year== dd.m_year
    )
    {
        return true; 
    }
    return false; 
}

