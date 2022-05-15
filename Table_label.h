#ifndef TABLE_LABEL_H
#define TABLE_LABEL_H

#include <iostream>
#include <cstring>

#include "Label.h"

class Table_label {
    Label *pointer;
    int size;
public:
    int top;

    Table_label(int max_size);

    ~Table_label();

    Label &operator[](int k);

    void print();

    int put(const char *buf);

    bool check_TABLE_LABLE();
};

#endif //TABLE_LABEL_H