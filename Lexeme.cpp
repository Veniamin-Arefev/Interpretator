#include "Lexeme.h"


Lexeme::Lexeme(type_of_lexeme type, int value) : lexeme_type(type), lexeme_value(value) {}

Lexeme::Lexeme(type_of_lexeme type, char *temp) : lexeme_type(type) {
    lexeme_string = new char[strlen(temp) + 2];
    strcpy(lexeme_string, temp);
}

type_of_lexeme Lexeme::get_type() const {
    return lexeme_type;
}

char *Lexeme::get_string() {
    return lexeme_string;
}

int Lexeme::get_value() const {
    return lexeme_value;
}

std::ostream &operator<<(std::ostream &s, Lexeme lexeme) {
    s << "(" << get_type_string_by_id(lexeme.lexeme_type);
    if (lexeme.lexeme_type == LEX_ID) {
        s << " name: " << TABLE_IDENTIFIER[lexeme.lexeme_value].get_name() << " type: "
          << TABLE_IDENTIFIER[lexeme.lexeme_value].get_type();
    } else if ((lexeme.lexeme_type == LEX_STRING && lexeme.lexeme_value != 15) || (lexeme.lexeme_type == LEX_COLON)) {
        s << ", '" << TABLE_STRING[lexeme.lexeme_value] << "'";
    } else {
        s << ", " << lexeme.lexeme_value;
    }
    s << ");" << std::endl;
    return s;
}