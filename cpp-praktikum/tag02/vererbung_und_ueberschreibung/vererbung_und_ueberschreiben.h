#ifndef PERSON_H
#define PERSON_H

#include <string>

// Elternklasse definieren 
class Person
{
private: 
    std::string m_sName; 
    std::string sSurname; 
    int m_iAge; 
public: 
    Person(std::string name, std::string surname, int age); 
    ~Person(); 
    std::string getName() const; 
    std::string getSurname() const; 
    int getAge() const; 
    bool setAge(int age); 
}; 


// Kinder Klasse definieren 
class Student : public Person
{
private:
    unsigned int m_uiStudentID; 
public: 
    Student(std::string name, std::string surname, int age, unsigned int studentid); 
    ~Student(); 
    unsigned int getStudentID() const; 
    std::string getName() const; 
}; 



#endif

