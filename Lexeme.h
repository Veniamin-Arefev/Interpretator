#ifndef LEXEME_H
#define LEXEME_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stack>

#include "Type_of_lexeme.h"
#include "Table_identifier.h"
#include "Table_string.h"

extern Table_identifier TABLE_IDENTIFIER;
extern Table_string TABLE_STRING;

class Lexeme {
    type_of_lexeme lexeme_type;
    int lexeme_value = 0;
    char *lexeme_string = nullptr;
public:
    Lexeme(type_of_lexeme type = LEX_NULL, int value = 0);
    Lexeme(type_of_lexeme type, char *temp);

    type_of_lexeme get_type() const;
    int get_value() const;
    char *get_string();

    friend std::ostream &operator<<(std::ostream &s, Lexeme lexeme);
};

#endif //LEXEME_H
