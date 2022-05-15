#include <string>

#include "Parser.h"
#include "Interpretator.h"

void Parser::print_error_lexeme(const Lexeme &lexeme, const std::string &string) const {
    std::cout << "\x1b[37;41m" << "Line: " << scanner.count_str << " Column: " << scanner.count_col << " Unexpected lexeme"
              << lexeme << string << "\x1b[0m" << std::endl;
}

void Parser::print_error_char_pointer(const char *pointer) const {
    std::cout << "\x1b[37;41m" << "Line: " << scanner.count_str << " Column: " << scanner.count_col << pointer << "\x1b[0m"
              << std::endl;
}

void Parser::print_error_char(const char in_char, const std::string &string) const {
    std::cout << "\x1b[37;41m" << "Line: " << scanner.count_str << " Column: " << scanner.count_col << " Unexpected symbol "
              << in_char << string << "\x1b[0m" << std::endl;
}

void Parser::START() {
    try {
        if (current_type == LEX_PROGRAM) {
            get_next_lexeme();
        } else {
            throw current_lexeme;
        }
        if (current_type == LEX_LBRACKET) {
            get_next_lexeme();
            VARS_TYPE();
            START1();
        } else {
            throw current_lexeme;
        }
        if (current_type != LEX_RBRACKET) {
            throw current_lexeme;
        } else {
            get_next_lexeme();
            if (current_type != LEX_EOF) {
                throw " Program finished with ERRORS ";
            }
        }
        if (!TABLE_LABEL.check_TABLE_LABLE()) {
            throw " Some LABELS weren't identificated ";
        }
    }
    catch (Lexeme &lexeme) {
        print_error_lexeme(lexeme, "Wrong rule for program");
        error_bit = true;
    }
    catch (const char *pointer) {
        print_error_char_pointer(pointer);
        error_bit = true;
    }
}

void Parser::VARS_TYPE() {
    try {
        while ((current_type == LEX_INT) || (current_type == LEX_BOOLEAN) || (current_type == LEX_STRING)) {
            VARS();
            if (current_type != LEX_SEMICOLON) {
                throw current_lexeme;
            }
            program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
            get_next_lexeme();
        }
    }
    catch (Lexeme &lexeme) {
        print_error_lexeme(lexeme, "Wrong rule for initialization");
        error_bit = true;
        get_next_lexeme();
    }
    catch (const char *pointer) {
        print_error_char_pointer(pointer);
        error_bit = true;
        get_next_lexeme();
    }
}

void Parser::VARS() {
    type_of_lexeme tmp = current_type;
    if (current_type == LEX_INT || current_type == LEX_BOOLEAN || current_type == LEX_STRING) {
        get_next_lexeme();
        st_lex.push(tmp);
        if (current_type == LEX_ID) {
            TABLE_IDENTIFIER[current_val].set_declare();
            TABLE_IDENTIFIER[current_val].set_type(tmp);
        }
        VAR();
        while (current_type == LEX_COMMA) {
            get_next_lexeme();
            st_lex.push(tmp);
            try {
                if (current_type == LEX_ID) {
                    if (TABLE_IDENTIFIER[current_val].get_declare() == true)
                        throw " Twise initializated ";
                    TABLE_IDENTIFIER[current_val].set_declare();
                    TABLE_IDENTIFIER[current_val].set_type(tmp);
                }
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                while (current_type != LEX_COMMA)
                    get_next_lexeme();
            }
            VAR();
        }
    } else
        throw current_lexeme;
}

void Parser::VAR() {
    int tmp_c_val = current_val;
    if (current_type == LEX_ID) {
        st_int.push(current_val);
        get_next_lexeme();
        if (current_type == LEX_ASSIGN) {
            st_lex.push(TABLE_IDENTIFIER[tmp_c_val].get_type());
            st_lex.push(LEX_ID);
            program_poliz.push_back(Lexeme(POLIZ_ADDRESS, tmp_c_val));
            tmp_c_val = current_val;
            get_next_lexeme();
            CONSTANT();
            program_poliz.push_back(Lexeme(LEX_ASSIGN, LEX_ASSIGN));
        }
    } else {
        if (current_type == LEX_COMMA || current_type == LEX_SEMICOLON) {}
        else
            throw current_lexeme;
    }
}

void Parser::CONSTANT() {
    if (current_type == LEX_STRING) {
        st_lex.push(LEX_STRING);
        equals_type();
        program_poliz.push_back(current_lexeme);
        get_next_lexeme();
    } else if (current_type == LEX_TRUE) {
        st_lex.push(LEX_BOOLEAN);
        equals_type();
        program_poliz.push_back(Lexeme(LEX_TRUE, LEX_TRUE));
        get_next_lexeme();
    } else if (current_type == LEX_FALSE) {
        st_lex.push(LEX_BOOLEAN);
        equals_type();
        program_poliz.push_back(Lexeme(LEX_FALSE, LEX_FALSE));
        get_next_lexeme();
    } else if (current_type == LEX_NUMBER) {
        st_lex.push(LEX_INT);
        equals_type();
        program_poliz.push_back(current_lexeme);
        get_next_lexeme();
    } else {
        throw current_lexeme;
    }
}

void Parser::START1() {
    while (current_type != LEX_RBRACKET && current_type != LEX_EOF) {
        STATEMENT();
    }
}

void Parser::STATEMENT() {
    try {
        int pl0, pl1, pl2, pl3;
        if (current_type == LEX_RBRACKET) {

        } else if (current_type == LEX_IF) {
            try {
                get_next_lexeme();
                EXPRESSION_OR();
                equals_bool();
                pl2 = program_poliz.get_free();
                program_poliz.blank();
                program_poliz.push_back((Lexeme(POLIZ_IF_GO, POLIZ_IF_GO)));
                if (current_type == LEX_LBRACKET) {
                    get_next_lexeme();
                    STATEMENT();
                    while (current_type == LEX_SEMICOLON) {
                        program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
                        get_next_lexeme();
                        STATEMENT();
                    }
                    pl3 = program_poliz.get_free();
                    if (current_type != LEX_RBRACKET) {
                        throw current_lexeme;
                    }

                    get_next_lexeme();
                    if (current_type == LEX_ELSE) {
                        get_next_lexeme();
                        if (current_type == LEX_LBRACKET) {
                            program_poliz.blank();
                            program_poliz.push_back(Lexeme(POLIZ_GO, POLIZ_GO));
                            program_poliz.push_back(Lexeme(POLIZ_LABEL, program_poliz.get_free()), pl2);
                            get_next_lexeme();
                            STATEMENT();
                            while (current_type == LEX_SEMICOLON) {
                                program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
                                get_next_lexeme();
                                STATEMENT();
                            }
                            program_poliz.push_back(Lexeme(POLIZ_LABEL, program_poliz.get_free()), pl3);
                        }
                        if (current_type != LEX_RBRACKET) {
                            throw current_lexeme;
                        }
                        get_next_lexeme();
                    } else {
                        program_poliz.push_back(Lexeme(POLIZ_LABEL, program_poliz.get_free()), pl2);
                        STATEMENT();
                    }
                } else throw current_lexeme;
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for IF ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for IF ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_WHILE) {
            try {
                pl0 = program_poliz.get_free();
                get_next_lexeme();
                EXPRESSION_OR();
                equals_bool();
                pl1 = program_poliz.get_free();
                program_poliz.blank();
                program_poliz.push_back(Lexeme(POLIZ_IF_GO, POLIZ_IF_GO));
                if (current_type == LEX_LBRACKET) {
                    get_next_lexeme();
                    STATEMENT();
                    while (current_type == LEX_SEMICOLON) {
                        program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
                        get_next_lexeme();
                        STATEMENT();
                    }
                    program_poliz.push_back(Lexeme(POLIZ_LABEL, pl0));
                    program_poliz.push_back(Lexeme(POLIZ_GO, POLIZ_GO));
                    program_poliz.push_back(Lexeme(POLIZ_LABEL, program_poliz.get_free()), pl1);
                    if (current_type != LEX_RBRACKET)
                        throw current_lexeme;
                    get_next_lexeme();
                    STATEMENT();
                } else
                    throw current_lexeme;
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for WHILE ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for WHILE ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_DO) {
            try {
                get_next_lexeme();
                pl0 = program_poliz.get_free();
                if (current_type == LEX_LBRACKET) {
                    get_next_lexeme();
                    STATEMENT();
                    while (current_type == LEX_SEMICOLON) {
                        program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
                        get_next_lexeme();
                        STATEMENT();
                    }
                    if (current_type == LEX_RBRACKET) {
                        get_next_lexeme();
                    } else {
                        throw current_lexeme;
                    }
                } else
                    throw current_lexeme;
                if (current_type == LEX_WHILE) {
                    get_next_lexeme();
                    if (current_type == LEX_LPARENT) {
                        EXPRESSION_OR();
                        equals_bool();
                        pl1 = program_poliz.get_free();
                        program_poliz.push_back(Lexeme(POLIZ_LABEL, pl1 + 4));
                        program_poliz.push_back(Lexeme(POLIZ_IF_GO, POLIZ_IF_GO));
                        program_poliz.push_back(Lexeme(POLIZ_LABEL, pl0));
                        program_poliz.push_back(Lexeme(POLIZ_GO, POLIZ_GO));
                        if (current_type != LEX_SEMICOLON)
                            throw current_lexeme;
                        program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
                        get_next_lexeme();
                        STATEMENT();
                    } else {
                        throw current_lexeme;
                    }
                } else {
                    throw current_lexeme;
                }
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for DO_WHILE ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for DO_WHILE ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_READ) {
            try {
                get_next_lexeme();
                if (current_type == LEX_LPARENT) {
                    get_next_lexeme();
                    if (current_type == LEX_ID) {
                        check_id_in_read();
                        program_poliz.push_back(Lexeme(POLIZ_ADDRESS, current_val));
                        get_next_lexeme();
                    } else
                        throw current_lexeme;
                    if (current_type == LEX_RPARENT) {
                        get_next_lexeme();
                        program_poliz.push_back(Lexeme(LEX_READ, LEX_READ));
                    } else
                        throw current_lexeme;
                } else
                    throw current_lexeme;
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for READ ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for READ ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_WRITE) {
            try {
                get_next_lexeme();
                if (current_type == LEX_LPARENT) {
                    get_next_lexeme();
                    EXPRESSION_OR();
                    while (current_type == LEX_COMMA) {
                        get_next_lexeme();
                        EXPRESSION_OR();
                    }
                    if (current_type == LEX_RPARENT) {
                        program_poliz.push_back(Lexeme(LEX_WRITE, LEX_WRITE));
                        get_next_lexeme();
                    } else
                        throw current_lexeme;
                } else
                    throw current_lexeme;
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for WRITE ");
                error_bit = true;
                while (current_type != LEX_QUOTE && current_type != LEX_SEMICOLON && current_type != EOF &&
                       current_type != LEX_RPARENT) {
                    get_next_lexeme();
                }
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                while (current_type != LEX_QUOTE && current_type != LEX_SEMICOLON && current_type != EOF) {
                    get_next_lexeme();
                }
            }
            catch (char c) {
                print_error_char(c, " \n Wrong rule for WRITE ");
                error_bit = true;
                while (c != '\"' && c != ';' && c != EOF && c != ')') {
                    c = scanner.gc();
                }
            }
        } else if (current_type == LEX_ID) {
            try {
                if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
                    throw " Not declared ";
                }
                type_of_lexeme tmp = TABLE_IDENTIFIER[current_val].get_type();
                st_lex.push(tmp);
                st_lex.push(LEX_ID);
                program_poliz.push_back(Lexeme(POLIZ_ADDRESS, current_val));

                get_next_lexeme();
                if (current_type == LEX_ASSIGN) {
                    get_next_lexeme();
                    EXPRESSION_OR();
                    equals_type();
                    program_poliz.push_back(Lexeme(LEX_ASSIGN, LEX_ASSIGN));
                } else if (current_type == LEX_PLUS_EQUALS) {
                    if (tmp == LEX_BOOLEAN) throw "you can't do uno plus with bool";
                    if (tmp != LEX_STRING) {
                        get_next_lexeme();
                        EXPRESSION_OR();
                        equals_type();
                        program_poliz.push_back(Lexeme(LEX_PLUS_EQUALS, LEX_PLUS_EQUALS));
                    } else
                        throw "you can't do uno plus with string";
                } else if (current_type == LEX_MINUS_EQUALS) {
                    if (tmp == LEX_BOOLEAN) throw "you can't do uno minus with bool";
                    if (tmp != LEX_STRING) {
                        get_next_lexeme();
                        EXPRESSION_OR();
                        equals_type();
                        program_poliz.push_back(Lexeme(LEX_MINUS_EQUALS, LEX_MINUS_EQUALS));
                    } else
                        throw "you can't do uno minus with string";
                } else
                    throw current_lexeme;
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for ASSIGN ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                while (current_type != LEX_SEMICOLON) {
                    get_next_lexeme();
                }
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for ASSIGN ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_SEMICOLON) {
            program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
            get_next_lexeme();
        } else if (current_type == LEX_GOTO) {
            try {
                int i;
                char *g = scanner.get_goto_label();
                for (i = 0; i < TABLE_LABEL.top; i++) {
                    if (!strcmp(TABLE_LABEL[i].get_name(), g)) {
                        throw "Unknown label";
                    }
                    if (!strcmp(TABLE_IDENTIFIER[i].get_name(), g)) {
                        throw "This is a variable, can't be label!";
                    }
                }
                if (i == TABLE_LABEL.top) {
                    TABLE_LABEL.put(g);
                }
                program_poliz.push_back(Lexeme(LEX_GOTO, i));
                program_poliz.push_back(Lexeme(POLIZ_GO, POLIZ_GO));
                get_next_lexeme();
                if (current_type != LEX_SEMICOLON) {
                    throw "Expected semicolon";
                }
                program_poliz.push_back(Lexeme(LEX_SEMICOLON, LEX_SEMICOLON));
                get_next_lexeme();
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for GOTO ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for GOTO ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_MINUS) {
            try {
                get_next_lexeme();
                if (current_type == LEX_ID) {
                    check_id();
                    if (TABLE_IDENTIFIER[current_val].get_type() != LEX_INT) {
                        throw " MINUS: WRONG TYPE WITH UN_MINUS ";
                    }
                    EXPRESSION_OR();
                    equals_type();
                    get_next_lexeme();
                }
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for GOTO ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for GOTO ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_PLUS) {
            try {
                get_next_lexeme();
                if (current_type == LEX_ID) {
                    check_id();
                    if (TABLE_IDENTIFIER[current_val].get_type() != LEX_INT) {
                        throw " MINUS: WRONG TYPE WITH UN_MINUS ";
                    }
                    EXPRESSION_OR();
                    equals_type();
                    get_next_lexeme();
                }
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for GOTO ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for GOTO ");
                error_bit = true;
                get_next_lexeme();
            }
        } else if (current_type == LEX_COLON) {
            try {
                char *g = current_str;
                int pol = program_poliz.get_free(), i;
                for (i = 0; i < TABLE_LABEL.top; i++) {
                    if (!strcmp(TABLE_LABEL[i].get_name(), g)) {
                        if (TABLE_LABEL[i].get_declare()) {
                            throw "More than one declare of label";
                        } else {
                            break;
                        }
                    }
                }
                if (i == TABLE_LABEL.top) {
                    TABLE_LABEL.put(current_str);
                }
                TABLE_LABEL[i].set_position(pol);
                TABLE_LABEL[i].set_declare();
                get_next_lexeme();
            }
            catch (Lexeme &lexeme) {
                print_error_lexeme(lexeme, " Wrong rule for COLON ");
                error_bit = true;
                get_next_lexeme();
            }
            catch (const char *pointer) {
                print_error_char_pointer(pointer);
                error_bit = true;
                get_next_lexeme();
            }
            catch (char c) {
                print_error_char(c, " Wrong rule for COLON ");
                error_bit = true;
                get_next_lexeme();
            }
        } else {
            throw current_lexeme;
        }
    }
    catch (Lexeme &lexeme) {
        print_error_lexeme(lexeme, " Wrong rule for EXPRESSION ");
        error_bit = true;
        get_next_lexeme();
        while ((current_type != LEX_SEMICOLON) && (current_type != LEX_RBRACKET) && (current_type != LEX_EOF)) {
            get_next_lexeme();
        }
    }
    catch (const char *pointer) {
        print_error_char_pointer(pointer);
        error_bit = true;
        get_next_lexeme();
        while ((current_type != LEX_SEMICOLON) && (current_type != LEX_RBRACKET) && (current_type != LEX_EOF)) {
            get_next_lexeme();
        }
    }
    catch (char c) {
        print_error_char(c, " Wrong rule for EXPRESSION ");
        error_bit = true;
        get_next_lexeme();
        while ((current_type != LEX_SEMICOLON) && (current_type != LEX_RBRACKET) && (current_type != LEX_EOF)) {
            get_next_lexeme();
        }
    }
}

void Parser::EXPRESSION_OR() {
    EXPRESSION_AND();
    while (current_type == LEX_OR) {
        st_lex.push(current_type);
        get_next_lexeme();
        if (current_type == LEX_ID) {
            if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
                throw " Not declared ";
            }
        }
        EXPRESSION_OR();
        check_op();
    }
}

void Parser::EXPRESSION_AND() {
    EXPRESSION();
    while (current_type == LEX_AND) {
        st_lex.push(current_type);
        get_next_lexeme();
        if (current_type == LEX_ID) {
            if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
                throw " Not declared ";
            }
        }
        EXPRESSION();
        check_op();
    }
}

void Parser::EXPRESSION() {
    EXPRESSION1();
    if (current_type == LEX_EQUALS || current_type == LEX_LESSER || current_type == LEX_GREATER ||
        current_type == LEX_LESSER_EQUALS ||
        current_type == LEX_GREATER_EQUALS || current_type == LEX_NOT_EQUALS || current_type == LEX_ASSIGN ||
        current_type == LEX_PLUS_EQUALS ||
        current_type == LEX_MINUS_EQUALS) {
        st_lex.push(current_type);
        get_next_lexeme();
        if (current_type == LEX_ID) {
            if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
                throw " Not declared ";
            }
        }
        EXPRESSION1();
        check_op();
    }
}

void Parser::EXPRESSION1() {
    type_of_lexeme tmp;
    ARITHMETIC1();
    while (current_type == LEX_PLUS || current_type == LEX_MINUS || current_type == LEX_OR) {
        st_lex.push(current_type);
        get_next_lexeme();
        if (current_type == LEX_MINUS || current_type == LEX_PLUS) {
            tmp = current_type;
            get_next_lexeme();
            if (current_type == LEX_ID) {
                if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
                    throw " Not declared ";
                }
                if (TABLE_IDENTIFIER[current_val].get_type() == LEX_INT) {
                    program_poliz.push_back(Lexeme(LEX_NUMBER, 0));
                    program_poliz.push_back(Lexeme(LEX_ID, current_val));
                    st_lex.push(current_type);
                    program_poliz.push_back(Lexeme(tmp, 0));
                }
            } else if (current_type == LEX_NUMBER) {
                program_poliz.push_back(Lexeme(LEX_NUMBER, 0));
                ARITHMETIC2();
                program_poliz.push_back(Lexeme(tmp, 0));
            } else {
                throw current_lexeme;
            }
        } else {
            if (current_type == LEX_ID) {
                if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
                    throw " Not declared ";
                }
            }
            ARITHMETIC1();
        }
        check_op();
    }
}

void Parser::ARITHMETIC1() {
    ARITHMETIC2();
    while (current_type == LEX_MULTIPLY || current_type == LEX_DIV || current_type == LEX_AND ||
           current_type == LEX_MOD) {
        st_lex.push(current_type);
        get_next_lexeme();
        if (current_type == LEX_ID) {
            if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
                throw " Not declared ";
            }
        }
        ARITHMETIC2();
        check_op();
    }
}


void Parser::ARITHMETIC2() {
    if (current_type == LEX_ID) {
        if (TABLE_IDENTIFIER[current_val].get_declare() == false) {
            throw " Not declared ";
        }
        type_of_lexeme tmp = TABLE_IDENTIFIER[current_val].get_type();
        st_lex.push(tmp);
        program_poliz.push_back(Lexeme(current_lexeme));
        get_next_lexeme();
    } else if (current_type == LEX_STRING) {
        st_lex.push(LEX_STRING);
        program_poliz.push_back(current_lexeme);
        get_next_lexeme();
    } else if (current_type == LEX_NUMBER) {
        st_lex.push(LEX_INT);
        program_poliz.push_back(current_lexeme);
        get_next_lexeme();
    } else if (current_type == LEX_TRUE) {
        st_lex.push(LEX_BOOLEAN);
        program_poliz.push_back(Lexeme(LEX_TRUE, LEX_TRUE));
        get_next_lexeme();
    } else if (current_type == LEX_FALSE) {
        st_lex.push(LEX_BOOLEAN);
        program_poliz.push_back(Lexeme(LEX_FALSE, LEX_FALSE));
        get_next_lexeme();
    } else if (current_type == LEX_NOT) {
        get_next_lexeme();
        ARITHMETIC2();
        check_not();
    } else if (current_type == LEX_LPARENT) {
        get_next_lexeme();
        EXPRESSION();
        if (current_type == LEX_RPARENT)
            get_next_lexeme();
        else {
            throw current_lexeme;
        }
    } else
        throw current_lexeme;
}

void get_lexeme_from_stack(std::stack<type_of_lexeme> &stack, type_of_lexeme &memory) {
    memory = stack.top();
    stack.pop();
}

void Parser::get_next_lexeme() {
    current_lexeme = scanner.get_lexeme();
    if (Interpretator::debug_mode) {
        std::cout << current_lexeme;
    }
    current_type = current_lexeme.get_type();
    current_val = current_lexeme.get_value();
    current_str = current_lexeme.get_string();
}

Parser::Parser(const char *program) : scanner(program), program_poliz(POLIZ_MAX_SIZE) {}

void Parser::analyze() {
    if (Interpretator::debug_mode) {
        std::cout << Interpretator::debug_separator << " SCANNER " << Interpretator::debug_separator << std::endl;
    }
    get_next_lexeme();
    START();
    if (!is_error()) {
        if (Interpretator::debug_mode) {
            std::cout << Interpretator::debug_separator << " POLIZ " << Interpretator::debug_separator << std::endl;
            program_poliz.print();
        }
    }
}

void Parser::check_id() {
    if (TABLE_IDENTIFIER[current_val].get_declare() == true)
        st_lex.push(TABLE_IDENTIFIER[current_val].get_type());
    else
        throw "Not declared";
}

void Parser::check_op() {
    type_of_lexeme t1, t2, op, result = LEX_BOOLEAN;
    get_lexeme_from_stack(st_lex, t2);
    get_lexeme_from_stack(st_lex, op);
    get_lexeme_from_stack(st_lex, t1);
    if (t1 == t2) {
        if (t1 == LEX_INT) {
            if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_MULTIPLY || op == LEX_DIV || op == LEX_MOD) {
                result = LEX_INT;
            } else if (op == LEX_AND || op == LEX_OR) {
                throw " Wrong types in op ";
            } else {
                result = LEX_BOOLEAN;
            }
        }
        if (t1 == LEX_BOOLEAN) {
            if (op == LEX_AND || op == LEX_OR || op == LEX_EQUALS || op == LEX_NOT_EQUALS) {
                result = LEX_BOOLEAN;
            } else {
                throw " Wrong types in op ";
            }
        }
        if (t1 == LEX_STRING) {
            if (op == LEX_PLUS) {
                result = LEX_STRING;
            } else if (op == LEX_EQUALS || op == LEX_NOT_EQUALS || op == LEX_GREATER || op == LEX_LESSER ||
                       op == LEX_GREATER_EQUALS || op == LEX_LESSER_EQUALS) {
                result = LEX_BOOLEAN;
            } else {
                throw " Wrong types in op ";
            }
        }
        st_lex.push(result);
        if (op >= scanner.get_words_list_size()) {
            program_poliz.push_back(Lexeme(op, op % scanner.get_words_list_size() + 1));
        } else
            program_poliz.push_back(Lexeme(op, op));
    } else {
        throw " Wrong types in op ";
    }
}

void Parser::check_not() {
    if (st_lex.top() != LEX_BOOLEAN)
        throw "Wrong type is in not";
    else
        program_poliz.push_back(Lexeme(LEX_NOT, LEX_NOT));
}

void Parser::equals_type() {
    type_of_lexeme t = st_lex.top();
    st_lex.pop();
    if (st_lex.top() != LEX_ID)
        throw "Wrong types are in :=";
    st_lex.pop();
    if (t != st_lex.top())
        throw "Wrong types are in :=";
    st_lex.pop();
}

void Parser::equals_bool() {
    if (st_lex.top() != LEX_BOOLEAN)
        throw "Expression is not boolean";
    st_lex.pop();
}

void Parser::check_id_in_read() const {
    if (!TABLE_IDENTIFIER[current_val].get_declare())
        throw "Not declared";
}

Poliz *Parser::get_poliz() {
    return &program_poliz;
}

bool Parser::is_error() const {
    return error_bit || scanner.is_error();
}
