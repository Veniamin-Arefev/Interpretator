#ifndef PARSER_H
#define PARSER_H

#include <stack>

#include "Table_label.h"
#include "Poliz.h"
#include "Scanner.h"

extern Table_identifier TABLE_IDENTIFIER;
extern Table_string TABLE_STRING;
extern Table_label TABLE_LABEL;

class Parser {
    bool error_bit = false;

    Scanner scanner;
    Poliz program_poliz;

    std::stack<int> st_int;
    std::stack<type_of_lexeme> st_lex;

    Lexeme current_lexeme;
    type_of_lexeme current_type;
    int current_val;
    char *current_str;
public:
    void CONSTANT();
    void VAR();
    void START();
    void VARS_TYPE();
    void VARS();
    void START1();
    void STATEMENT();
    void EXPRESSION();
    void EXPRESSION1();
    void ARITHMETIC1();
    void ARITHMETIC2();
    void EXPRESSION_OR();
    void EXPRESSION_AND();
    void check_id();
    void check_op();
    void check_not();
    void equals_type();
    void equals_bool();
    void check_id_in_read() const;

    void get_next_lexeme();

    Parser(const char *program);

    void analyze();

    bool is_error() const;
    Poliz *get_poliz();

    void print_error_lexeme(const Lexeme &lexeme, const std::string &string) const;
    void print_error_char_pointer(const char *pointer) const;
    void print_error_char(const char in_char, const std::string &string) const;
};

#endif //PARSER_H
