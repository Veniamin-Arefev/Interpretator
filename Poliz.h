#ifndef POLIZ_H
#define POLIZ_H

#include "Lexeme.h"
#include <iostream>

class Poliz {
    Lexeme *p;
    int size;
    int current_index;
public:
    Poliz(int max_size);
    ~Poliz();

    void push_back(Lexeme l);
    void push_back(Lexeme l, int place);
    int get_free();
    void blank();
    Lexeme &operator[](int index);

    void print();
};

#endif //POLIZ_H
