#include "Executor.h"

inline void get_int_from_top(std::stack<int> &stack, int &memory) {
    memory = stack.top();
    stack.pop();
}

inline void get_string_from_top(std::stack<std::string> &stack, std::string &memory) {
    memory = stack.top();
    stack.pop();
}

void Executor::execute(Poliz &program) {
    std::stack<int> arguments;
    std::stack<int> arguments_reverse;
    std::stack<std::string> arguments_string;
    std::stack<std::string> arguments_string_reverse;
    int index = -1, size = program.get_free();
    //for use in cases
    int int1, int2, int3;
    std::string string1, string2;

    while (++index < size) {
        current_poliz_lexem = program[index];
        switch (current_poliz_lexem.get_type()) {
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_NUMBER:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                arguments.push(current_poliz_lexem.get_value());
                break;
            case LEX_ID:
                int1 = current_poliz_lexem.get_value();
                if (TABLE_IDENTIFIER[int1].get_assign()) {
                    if (TABLE_IDENTIFIER[int1].get_type() == LEX_STRING) {
                        string1 = TABLE_STRING[TABLE_IDENTIFIER[int1].get_value()];
                        arguments_string.push(string1);
                    }
                    arguments.push(TABLE_IDENTIFIER[int1].get_value());
                } else {
                    throw "POLIZ: indefinite identifier";
                }
                break;
            case LEX_NOT:
                get_int_from_top(arguments, int1);
                arguments.push(!int1);
                break;
            case LEX_OR:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                arguments.push(int2 || int1);
                break;
            case LEX_AND:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                arguments.push(int2 && int1);
                break;
            case POLIZ_GO:
                get_int_from_top(arguments, int1);
                index = int1 - 1;
                break;
            case POLIZ_IF_GO:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (!int2) index = int1 - 1;
                break;
            case LEX_GOTO:
                int1 = current_poliz_lexem.get_value();
                arguments.push(TABLE_LABEL[int1].get_position());
                break;
            case LEX_WRITE:
                while (!arguments_string.empty()) {
                    get_string_from_top(arguments_string, string1);
                    arguments.pop();
                    arguments_string_reverse.push(string1);
                }
                while (!arguments.empty()) {
                    get_int_from_top(arguments, int2);
                    arguments_reverse.push(int2);
                }
                while (!arguments_string_reverse.empty()) {
                    get_string_from_top(arguments_string_reverse, string1);
                    std::cout << string1 << ' ';
                }
                while (!arguments_reverse.empty()) {
                    get_int_from_top(arguments_reverse, int2);
                    std::cout << int2 << ' ';
                }
                break;
            case LEX_READ:
                int k;
                get_int_from_top(arguments, int1);
                std::cout << std::endl;
                if (TABLE_IDENTIFIER[int1].get_type() == LEX_INT) {
                    std::cin >> k;
                } else if (TABLE_IDENTIFIER[int1].get_type() == LEX_STRING) {
                    std::cin >> string1;
                    k = TABLE_STRING.put(string1);
                } else {
                    std::string temp_string;
                    while (true) {
                        std::cin >> temp_string;
                        if (temp_string != "true" && temp_string != "false") {
                            std::cout << "Error in input:true/false" << std::endl;
                            continue;
                        }
                        k = (temp_string == "true") ? 1 : 0;
                        break;
                    }
                }
                TABLE_IDENTIFIER[int1].set_value(k);
                TABLE_IDENTIFIER[int1].set_assign();
                break;
            case LEX_PLUS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (!arguments_string.empty()) {
                    get_string_from_top(arguments_string, string1);
                    get_string_from_top(arguments_string, string2);
                    arguments_string.push(string2 + string1);
                    arguments.push(TABLE_STRING.put(string2 + string1));
                } else {
                    arguments.push(int1 + int2);
                }
                break;
            case LEX_MULTIPLY:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                arguments.push(int1 * int2);
                break;
            case LEX_MINUS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                arguments.push(int2 - int1);
                break;
            case LEX_DIV:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (int1) {
                    arguments.push(int2 / int1);
                } else {
                    throw "POLIZ: divide by zero";
                }
                break;
            case LEX_MOD:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (int1) {
                    arguments.push(int2 % int1);
                } else {
                    throw "POLIZ: mod by zero";
                }
                break;
            case LEX_MINUS_EQUALS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                int3 = TABLE_IDENTIFIER[int2].get_value();
                TABLE_IDENTIFIER[int2].set_value(int3 - int1);
                TABLE_IDENTIFIER[int2].set_assign();
                break;
            case LEX_PLUS_EQUALS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                int3 = TABLE_IDENTIFIER[int2].get_value();
                TABLE_IDENTIFIER[int2].set_value(int3 + int1);
                TABLE_IDENTIFIER[int2].set_assign();
                break;
            case LEX_EQUALS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (!arguments_string.empty()) {
                    get_string_from_top(arguments_string, string1);
                    get_string_from_top(arguments_string, string2);
                    arguments.push(string1 == string2);
                } else {
                    arguments.push(int1 == int2);
                }
                break;
            case LEX_LESSER:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (!arguments_string.empty()) {
                    get_string_from_top(arguments_string, string1);
                    get_string_from_top(arguments_string, string2);
                    arguments.push(string2 < string1);
                } else {
                    arguments.push(int2 < int1);
                }
                break;
            case LEX_GREATER:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (!arguments_string.empty()) {
                    get_string_from_top(arguments_string, string1);
                    get_string_from_top(arguments_string, string2);
                    arguments.push(string2 > string1);
                } else {
                    arguments.push(int2 > int1);
                }
                break;
            case LEX_LESSER_EQUALS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                arguments.push(int2 <= int1);
                break;
            case LEX_GREATER_EQUALS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                arguments.push(int2 >= int1);
                break;
            case LEX_NOT_EQUALS:
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                if (!arguments_string.empty()) {
                    get_string_from_top(arguments_string, string1);
                    get_string_from_top(arguments_string, string2);
                    arguments.push(string1 != string2);
                } else {
                    arguments.push(int1 != int2);
                }
                break;
            case LEX_STRING:
                int1 = current_poliz_lexem.get_value();
                string1 = TABLE_STRING[int1];
                arguments_string.push(string1);
                arguments.push(int1);
                break;
            case LEX_ASSIGN:
                if (!arguments_string.empty()) {
                    get_string_from_top(arguments_string, string1);
                }
                get_int_from_top(arguments, int1);
                get_int_from_top(arguments, int2);
                TABLE_IDENTIFIER[int2].set_value(int1);
                TABLE_IDENTIFIER[int2].set_assign();
                break;
            case LEX_SEMICOLON:
                if (!arguments.empty()) {
                    throw "POLIZ: Too much elems";
                }
                break;
            default: {
                throw "POLIZ: unexpected elem";
            }
        }
    }
}
