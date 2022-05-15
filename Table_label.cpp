#include "Table_label.h"
#include "Interpretator_Limits.h"

Table_label TABLE_LABEL(TABLE_LABEL_MAX_SIZE);

Table_label::Table_label(int max_size) : size(max_size), top(0) {
    pointer = new Label[size];
}

Table_label::~Table_label() {
    delete[] pointer;
}

Label &Table_label::operator[](int k) {
    return pointer[k];
}

void Table_label::print() {
    for (int j = 1; j < top; j++) {
        std::cout << pointer[j].get_position();
    }
}

bool Table_label::check_TABLE_LABLE() {
    for (int i = 0; i < top; i++) {
        if (!pointer[i].get_declare()) {
            return false;
        }
    }
    return true;
}

int Table_label::put(const char *buf) {
    for (int j = 0; j < top; j++) {
        if (!strcmp(buf, pointer[j].get_name())) {
            return j;
        }
    }
    pointer[top++].set_name(buf);
    return top - 1;
}
