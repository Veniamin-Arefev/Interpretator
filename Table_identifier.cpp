#include "Table_identifier.h"
#include "Interpretator_Limits.h"

Table_identifier TABLE_IDENTIFIER(TABLE_IDENTIFIER_MAX_SIZE);

Table_identifier::Table_identifier(int max_size) : size(max_size), top(0) {
    pointer = new Identifier[size];
}

Table_identifier::~Table_identifier() {
    delete[] pointer;
}

Identifier &Table_identifier::operator[](int k) {
    return pointer[k];
}

void Table_identifier::print() {
    for (int j = 0; j < top; j++)
        std::cout << pointer[j].get_value();
}

int Table_identifier::put(const char *buf) {
    for (int j = 0; j < top; j++) {
        if (!strcmp(buf, pointer[j].get_name())) {
            return j;
        }
    }
    pointer[top++].set_name(buf);
    return top - 1;
}		