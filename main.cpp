#include <iostream>
#include <string>
#include <fstream>

#include "Interpretator.h"

int main(int argc, const char *argv[]) {
    bool debug_mode = false;
    int file_index = -1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug_mode = true;
        } else if (argv[i][0] != '-') {
            if (file_index == -1) {
                file_index = i;
            } else {
                std::cout << "\x1b[37;41m" << "You can specify only one input file" << "\x1b[0m" << std::endl;
                exit(0);
            }
        } else {
            std::cout << "\x1b[37;41m" << "Unexpected - flag " << "\x1b[0m" << std::endl;
            exit(0);
        }
    }
    if (file_index == -1) {
        std::cout << "\x1b[37;41m" << "Specify input file" << "\x1b[0m" << std::endl;
        exit(0);
    }
    try {
        std::ifstream file = std::ifstream(argv[file_index]);
        if (!file.is_open()) {
            std::cout << "\x1b[37;41m" << "The specified file could not be opened" << "\x1b[0m" << std::endl;
            exit(0);
        }
        file.close();
        std::cout << "\x1b[37;46m" << "Interpretating " << argv[file_index] << "..." << "\x1b[0m" << std::endl;
        Interpretator interpretator(argv[file_index], debug_mode);
        interpretator.interpretation();
    } catch (std::bad_alloc &) {
        std::cout << "\x1b[37;41m" << "OUT OF MEMORY" << "\x1b[0m" << std::endl;
    } catch (...) {
        std::cout << "\x1b[37;41m" << "Unexpected error" << "\x1b[0m" << std::endl;
    }
}
