#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include "Parser.h"
#include "Executor.h"

class Interpretator {
    Parser parser;
    Executor executor;

public:
    static bool debug_mode;
    static const std::string debug_separator;

    Interpretator(const char *program_file, bool debug_mode = false);

    void interpretation();
};

#endif //INTERPRETATOR_H
