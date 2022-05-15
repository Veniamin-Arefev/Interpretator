#include "Poliz.h"

Poliz::Poliz(int max_size) : size(max_size), current_index(0) {
    p = new Lexeme[size];
}

Poliz::~Poliz() {
    delete[] p;
}

void Poliz::push_back(Lexeme l) {
    p[current_index++] = l;
}

void Poliz::push_back(Lexeme l, int place) {
    p[place] = l;
}

void Poliz::blank() {
    ++current_index;
}

int Poliz::get_free() {
    return current_index;
}

Lexeme &Poliz::operator[](int index) {
    if (index > size) {
        throw "POLIZ: out of array";
    } else if (index > current_index) {
        throw "POLIZ: undefined element of array";
    } else {
        return p[index];
    }
}

void Poliz::print() {
    for (int i = 0; i < current_index; ++i) {
        std::cout << p[i];
    }
}



