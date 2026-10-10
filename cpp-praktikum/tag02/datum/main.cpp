#include "datum.h"

int main()

{

    srand(time(nullptr)); 

    Date d1 = Date(2,2,2011); 
    // d1.m_day = 15; ist private, deshalb kann so nicht darauf zugegriffen werden 
    d1.setDay(15); 
    Date d2 = Date(15,2,2011); 
    bool tf = d2.isEqual(d1); 

    Date d3 = Date(14,2,2011); 
    Date d4 = Date(16,2,2011); 
    Date d5; 

    // Objekte in einen array verpacken 
    Date dates[5] = {d1, d2, d3, d4, d5}; 

    for (int i = 0; i < 5; i++)
    {
        std::cout << "date d"<< i+1 << " :"<<  dates[i].getDay() << "." << dates[i].getMonth() << "." << dates[i].getYear() << '\n'; 

        for (int j = i + 1; j < 5; j++)
        {
            std::cout << "d" << i+1 << ".compare(d" << j+1 << "): " << dates[i].compare(dates[j]) << '\n'; 
        }
    }

}

