#include <cstring>
#include "Label.h"

Label::Label() {}

char *Label::get_name() const{
    return name;
}

void Label::set_name(const char *n) {
    name = new char[strlen(n) + 1];
    strcpy(name, n);
}

bool Label::get_declare() const{
    return declare;
}

void Label::set_declare() {
    declare = true;
}

int Label::get_position() const{
    return position;
}

void Label::set_position(int p) {
    position = p;
}