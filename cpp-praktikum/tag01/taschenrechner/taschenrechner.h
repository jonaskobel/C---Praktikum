#ifndef TASCHENRECHNER_H
#define TASCHENRECHNER_H

#include <string>

double read_number(
    const std::string& text
);

char read_expression(
    const std::string& text
); 

double calculate(
    double a,
    double b,
    char expression
);

#endif