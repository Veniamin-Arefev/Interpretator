#ifndef TABLE_IDENTIFIER_H
#define TABLE_IDENTIFIER_H

#include <iostream>
#include <cstring>

#include "Identifier.h"

class Table_identifier {
    Identifier *pointer;
    int size;
    int top;
public:
    Table_identifier(int max_size);

    ~Table_identifier();

    Identifier &operator[](int k);

    int put(const char *buf);

    void print();
};


#endif //TABLE_IDENTIFIER_H
