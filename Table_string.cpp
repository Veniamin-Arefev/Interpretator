#include "Table_string.h"
#include "Interpretator_Limits.h"

Table_string TABLE_STRING(TABLE_STRING_MAX_SIZE);

Table_string::Table_string(int max_size) : size(max_size), top(0) {
    pointer = new std::string[size];
}

Table_string::~Table_string() {
    delete[] pointer;
}

std::string &Table_string::operator[](int k) {
    return pointer[k];
}

int Table_string::put(const char *buf) {
    pointer[top++].assign(buf, strlen(buf));
    return top - 1;
}

int Table_string::put(const std::string &s) {
    pointer[top++] = s;
    return top - 1;
}
