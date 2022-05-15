#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstring>

#include "Type_of_lexeme.h"

class Identifier {
    char *name;
    type_of_lexeme type;
    bool declare = false;
    bool assign = false;
    int value;
public:
    Identifier();

    char *get_name() const;
    void set_name(const char *n);

    type_of_lexeme get_type() const;
    void set_type(type_of_lexeme t);

    bool get_declare() const;
    void set_declare();

    bool get_assign() const;
    void set_assign();

    int get_value() const;
    void set_value(int v);
};

#endif //IDENTIFIER_H
