#include "vererbung_und_ueberschreiben.h"

Person::Person(std::string name, std::string surname, int age)
{
    m_sName = name; 
    sSurname = surname; 
    m_iAge = age; 
}
std::string Person::getName() const {
    return m_sName; 
}

std::string Person::getSurname() const {
    return sSurname; 
}

int Person::getAge() const {
    return m_iAge; 
}

bool Person::setAge(int age) {
    m_iAge = age;
    return 1; 
}

Student::Student(std::string name, std::string surname, int age, unsigned int studentid)
    : Person(name, surname, age)
{
    m_uiStudentID = studentid; 
}

unsigned int Student::getStudentID() const {
    return m_uiStudentID; 
}

std::string Student::getName() const {
    return "Student" + Person::getName();
}

Person::~Person() = default; 
Student::~Student() = default; 

