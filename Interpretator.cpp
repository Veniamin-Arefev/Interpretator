#include "Interpretator.h"

bool Interpretator::debug_mode;
const std::string Interpretator::debug_separator = std::string(20, '~');

Interpretator::Interpretator(const char *program_file, bool debug_mode) : parser(program_file) {
    Interpretator::debug_mode = debug_mode;
};

void Interpretator::interpretation() {
    parser.analyze();
    if (!parser.is_error()) {
        if (Interpretator::debug_mode) {
            std::cout << Interpretator::debug_separator << " EXECUTOR " << Interpretator::debug_separator << std::endl;
        }
        try {
            executor.execute(*parser.get_poliz());
            std::cout << "\x1b[37;42m" << "\nSUCCESS END OF PROGRAM" << "\x1b[0m" << std::endl;
        }
        catch (const char *c) {
            std::cout << "\x1b[37;41m" << c << "\x1b[0m" << std::endl;
        }
    }
}
