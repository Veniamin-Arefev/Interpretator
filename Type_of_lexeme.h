#ifndef TYPE_OF_LEXEME_H
#define TYPE_OF_LEXEME_H

enum type_of_lexeme {
    LEX_NULL, // 0
    LEX_AND, // 1
    LEX_BOOLEAN, // 2
    LEX_DO, // 3
    LEX_ELSE, // 4
    LEX_IF, // 5
    LEX_FALSE, // 6
    LEX_INT, // 7
    LEX_NOT, // 8
    LEX_OR, // 9
    LEX_PROGRAM, // 10
    LEX_READ, // 11
    LEX_TRUE, // 12
    LEX_WHILE, // 13
    LEX_WRITE, // 14
    LEX_STRING, // 15
    LEX_GOTO, // 16
    LEX_SEMICOLON, // 17
    LEX_COMMA, // 18
    LEX_ASSIGN, // 19
    LEX_LPARENT, // 20
    LEX_RPARENT, // 21
    LEX_LBRACKET, // 22
    LEX_RBRACKET, // 23
    LEX_EQUALS, // 24
    LEX_LESSER, // 25
    LEX_GREATER, // 26
    LEX_PLUS, // 27
    LEX_MINUS, // 28
    LEX_MULTIPLY, // 29
    LEX_DIV, // 30
    LEX_MOD, // 31
    LEX_FRONTSLASH, // 32
    LEX_BACKSLASH, // 33
    LEX_LESSER_EQUALS, // 34
    LEX_NOT_EQUALS, // 35
    LEX_GREATER_EQUALS, // 36
    LEX_PLUS_EQUALS, // 37
    LEX_MINUS_EQUALS, // 38
    LEX_QUOTE, //39
    LEX_COLON, // 40
    LEX_NUMBER, // 41
    LEX_ID, //42
    POLIZ_LABEL, // 43
    POLIZ_ADDRESS, // 44
    POLIZ_GO, // 45
    POLIZ_IF_GO, // 46
    LEX_EOF
};

const char *get_type_string_by_id(type_of_lexeme type);

#endif //TYPE_OF_LEXEME_H
