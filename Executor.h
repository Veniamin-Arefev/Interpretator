#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <cstring>

#include "Parser.h"

extern Table_identifier TABLE_IDENTIFIER;
extern Table_string TABLE_STRING;

class Executor {
    Lexeme current_poliz_lexem;
public:
    void execute(Poliz &program);
};

#endif //EXECUTOR_H
