#include "Identifier.h"

Identifier::Identifier(): type(LEX_NULL) {}

char *Identifier::get_name() const {
    return name;
}

void Identifier::set_name(const char *n) {
    name = new char[strlen(n) + 1];
    strcpy(name, n);
}

bool Identifier::get_declare() const {
    return declare;
}

void Identifier::set_declare() {
    declare = true;
}

bool Identifier::get_assign() const {
    return assign;
}

void Identifier::set_assign() {
    assign = true;
}

type_of_lexeme Identifier::get_type() const{
    return type;
}

void Identifier::set_type(type_of_lexeme t) {
    type = t;
}

int Identifier::get_value() const {
    return value;
}

void Identifier::set_value(int v) {
    value = v;
}

