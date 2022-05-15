#include "Type_of_lexeme.h"

const char *type_string[] = {"", "and", "boolean", "do", "else", "if", "false", "int", "not", "or", "program",
                             "read", "true", "while", "write", "string", "goto", ";", ",", "=", "(", ")", "{",
                             "}", "==", "<", ">", "+", "-", "*", "/", "%", "\\*", "*\\", "<=", "!=", ">=", "+=",
                             "-=", "\"", ":", "NUMBER", "ID", "LABEL", "&ID", "GO", "FGO", "EOF"};

const char *get_type_string_by_id(type_of_lexeme type) {
    return type_string[type];
}
