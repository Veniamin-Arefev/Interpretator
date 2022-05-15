#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>

#include "Lexeme.h"
#include "Table_string.h"
#include "Interpretator_Limits.h"

extern Table_identifier TABLE_IDENTIFIER;
extern Table_string TABLE_STRING;

class Scanner {
    enum state {
        H, IDENTIFIER, NUMBER, OPERATOR_EQUALS, NOT_EQUALS, EQUALS_EQUALS, QUOTE
    };
    state CURRENT_STATE = H;
    static const char *TABLE_WORDS[];
    static const type_of_lexeme words[];
    static const char *TABLE_DELIMITER[];
    static const type_of_lexeme delimiters[];

    std::ifstream fin;
    char current_char;
    int buf_top;

    void clear_buf();
    void add();
    void push_back(char c);
    int look_up(const char *buf, const char **list);
    bool error_bit = false;
public:

    char buf[SCANNER_BUFFER_MAX_SIZE];

    char gc();
    int count_str = 1; // счётчик строк
    int count_col = 1; // счётчик столбцов

    Scanner(const char *program);

    Lexeme get_lexeme();

    char *get_goto_label();

    int get_words_list_size() const;

    bool is_error() const;
};

#endif //SCANNER_H
