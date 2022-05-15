#include "Scanner.h"
#include "Interpretator.h"

const char *Scanner::TABLE_WORDS[] =
        {"", "and", "boolean", "do", "else", "if", "false", "int", "not", "or", "program", "read", "true", "while",
         "write", "string", "goto", nullptr};

const char *Scanner::TABLE_DELIMITER[] =
        {";", ",", "=", "(", ")", "{", "}", "==", "<", ">", "+", "-", "*", "/", "%", "\\*", "*\\", "<=", "!=", ">=",
         "+=", "-=", "\"", ":", nullptr};

const type_of_lexeme Scanner::words[] =
        {LEX_NULL, LEX_AND, LEX_BOOLEAN, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_NOT, LEX_OR, LEX_PROGRAM,
         LEX_READ, LEX_TRUE, LEX_WHILE, LEX_WRITE, LEX_STRING, LEX_GOTO, LEX_NULL};

const type_of_lexeme Scanner::delimiters[] =
        {LEX_SEMICOLON, LEX_COMMA, LEX_ASSIGN, LEX_LPARENT, LEX_RPARENT, LEX_LBRACKET, LEX_RBRACKET, LEX_EQUALS,
         LEX_LESSER, LEX_GREATER, LEX_PLUS, LEX_MINUS, LEX_MULTIPLY, LEX_DIV, LEX_MOD, LEX_FRONTSLASH, LEX_BACKSLASH,
         LEX_LESSER_EQUALS, LEX_NOT_EQUALS, LEX_GREATER_EQUALS, LEX_PLUS_EQUALS, LEX_MINUS_EQUALS, LEX_QUOTE, LEX_COLON,
         LEX_NULL};

int Scanner::get_words_list_size() const {
    return sizeof(Scanner::words) / sizeof(*Scanner::words);
}

char Scanner::gc() {
    if (fin.get(current_char)) {
        return current_char;
    } else {
        return EOF;
    }
}

void Scanner::push_back(char c) {
    fin.putback(c);
}

void Scanner::clear_buf() {
    buf_top = 0;
    memset(buf, '\0', sizeof(char) * SCANNER_BUFFER_MAX_SIZE);
}

void Scanner::add() {
    buf[buf_top++] = current_char;
}

Scanner::Scanner(const char *program) {
    fin = std::ifstream(program);
    clear_buf();
}

bool Scanner::is_error() const {
    return error_bit;
}

//list - TABLE_WORDS or TABLE_DELIMITER
int Scanner::look_up(const char *buf, const char **list) {
    int i = -1;
    while (list[++i]) {
        if (!strcmp(buf, list[i])) {
            return i;
        }
    }
    return 0;
}

// return label's index (after goto) in TABLE_LABEL
char *Scanner::get_goto_label() {
    current_char = gc();
    while (current_char == ' ' || current_char == '\n' || current_char == '\r' || current_char == '\t') {
        if (current_char == '\n') {
            count_str++;
            count_col = 1;
        }
        if (current_char == ' ' || current_char == '\t') {
            count_col++;
        }
        current_char = gc();
    }
    do {
        if (isalnum(current_char)) {
            add();
        } else if (current_char != ';') {
            if (current_char == '\n') {
                count_str++;
                count_col = 1;
                throw " \\n ";
            } else {
                throw current_char;
            }
        } else {
            break;
        }
        current_char = gc();
    } while (true);
    push_back(current_char);
    return buf;
}

Lexeme Scanner::get_lexeme() {
    int d = 0, temp_index;
    CURRENT_STATE = H;
    clear_buf();
    do {
        current_char = gc();
        if (current_char == EOF) {
            return Lexeme(LEX_EOF, LEX_EOF);
        }
        switch (CURRENT_STATE) {
            case H:
                if (isalpha(current_char)) {
                    add();
                    CURRENT_STATE = IDENTIFIER;
                } else if (isdigit(current_char)) {
                    d = current_char - '0';
                    CURRENT_STATE = NUMBER;
                } else {
                    switch (current_char) {
                        case '\r':
                            if (fin.peek() == '\n') {
                                gc();
                            }
                        case '\n':
                            count_str++;
                            count_col = 1;
                            break;
                        case ' ':
                        case '\t':
                            count_col++;
                            break;
                        case '<':
                        case '>':
                        case '+':
                        case '-':
                        case '*' :
                        case '/':
                        case '%':
                            add();
                            CURRENT_STATE = OPERATOR_EQUALS;
                            break;
                        case '!':
                            add();
                            CURRENT_STATE = NOT_EQUALS;
                            break;
                        case '=':
                            add();
                            CURRENT_STATE = EQUALS_EQUALS;
                            break;
                        case '"':
                            CURRENT_STATE = QUOTE;
                            break;
                        case ':':
                            CURRENT_STATE = IDENTIFIER;
                            push_back(current_char);
                            break;
                        case ';':
                            add();
                            temp_index = look_up(buf, TABLE_DELIMITER);
                            clear_buf();
                            count_col++;
                            return Lexeme(LEX_SEMICOLON, temp_index);
                        case ',':
                            add();
                            temp_index = look_up(buf, TABLE_DELIMITER);
                            clear_buf();
                            count_col++;
                            return Lexeme(LEX_COMMA, temp_index);
                        case '(':
                            add();
                            temp_index = look_up(buf, TABLE_DELIMITER);
                            clear_buf();
                            return Lexeme(LEX_LPARENT, temp_index);
                        case ')':
                            add();
                            temp_index = look_up(buf, TABLE_DELIMITER);
                            clear_buf();
                            return Lexeme(LEX_RPARENT, temp_index);
                        case '{':
                            add();
                            temp_index = look_up(buf, TABLE_DELIMITER);
                            clear_buf();
                            return Lexeme(LEX_LBRACKET, temp_index);
                        case '}':
                            add();
                            temp_index = look_up(buf, TABLE_DELIMITER);
                            clear_buf();
                            return Lexeme(LEX_RBRACKET, temp_index);
                        case '\\':
                            try {
                                current_char = gc();
                                if (current_char == '*') {
                                    if (Interpretator::debug_mode) {
                                        std::cout << "Entering comment " << std::endl;
                                    }
                                } else {
                                    throw current_char;
                                }
                                char c0 = gc();
                                current_char = gc();
                                //std::cout << current_char;
                                do {
                                    if ((c0 == '*') && (current_char == '\\'))
                                        break;
                                    if (Interpretator::debug_mode) {
                                        std::cout << c0;
                                    }
                                    c0 = current_char;
                                    current_char = gc();

                                } while (current_char != EOF);

                                if (current_char == EOF) {
                                    throw "Comment is't cloused";
                                }
                                std::cout << std::endl;
                                if (Interpretator::debug_mode) {
                                    std::cout << "Leaving comment" << std::endl;
                                }
                                CURRENT_STATE = H;
                                break;
                            }
                            catch (char c) {
                                std::cout << "\x1b[37;41m" << "Line: " << count_str << " Column: " << count_col
                                          << " Unexpected symbol" << c << "Wrong rule for comment" << "\x1b[0m"
                                          << std::endl;
                                error_bit = true;
                            }
                            catch (const char *c) {
                                std::cout << "\x1b[37;41m" << c << "\x1b[0m" << std::endl;
                                error_bit = true;
                            }
                            break;
                        default:
                            try {
                                add();
                                throw current_char;
                            }
                            catch (char c) {
                                //count_col++;
                                std::cout << "\x1b[37;41m" << "Line: " << count_str << " Column: " << count_col
                                          << " Unexpected symbol: " << c << "\x1b[0m" << std::endl;
                                error_bit = true;
                                //count_col--;
                            }
                            catch (const char *c) {
                                std::cout << "\x1b[37;41m" << c << "\x1b[0m" << std::endl;
                                error_bit = true;
                            }
                    }
                }
                break;
            case IDENTIFIER:
                if (isalpha(current_char) || isdigit(current_char)) {
                    add();
                } else if ((temp_index = look_up(buf, TABLE_WORDS)) > 0) {
                    push_back(current_char);
                    clear_buf();
                    return Lexeme(words[temp_index], temp_index);
                } else if (current_char == ':') {
                    count_col++;
                    if (buf_top == 0) {
                        throw " Expected name of COLON ";
                    }
                    return Lexeme(LEX_COLON, buf);
                } else {
                    temp_index = TABLE_IDENTIFIER.put(buf);
                    TABLE_IDENTIFIER[temp_index].set_value(temp_index);
                    push_back(current_char);
                    clear_buf();
                    return Lexeme(LEX_ID, temp_index);
                }
                break;
            case NUMBER:
                if (isdigit(current_char)) {
                    d = d * 10 + (current_char - '0');
                    count_col++;
                } else {
                    push_back(current_char);
                    clear_buf();
                    return Lexeme(LEX_NUMBER, d);
                }
                break;
            case OPERATOR_EQUALS:
                if (current_char == '=') {
                    add();
                    temp_index = look_up(buf, TABLE_DELIMITER);
                    clear_buf();
                    count_col++;
                    return Lexeme(delimiters[temp_index], temp_index);
                } else {
                    temp_index = look_up(buf, TABLE_DELIMITER);
                    push_back(current_char);
                    clear_buf();
                    count_col++;
                    return Lexeme(delimiters[temp_index], temp_index);
                }
            case NOT_EQUALS:
                if (current_char == '=') {
                    add();
                    temp_index = look_up(buf, TABLE_DELIMITER);
                    clear_buf();
                    count_col++;
                    return Lexeme(LEX_NOT_EQUALS, temp_index);
                } else {
                    std::cout << "Error with NOT EQUEAL";
                }
                break;
            case EQUALS_EQUALS: {
                if (current_char == '=') {
                    add();
                    temp_index = look_up(buf, TABLE_DELIMITER);
                    clear_buf();
                    count_col++;
                    return Lexeme(LEX_EQUALS, temp_index);
                } else {
                    temp_index = look_up(buf, TABLE_DELIMITER);
                    clear_buf();
                    count_col++;
                    push_back(current_char);
                    return Lexeme(LEX_ASSIGN, temp_index);
                }
            }
            case QUOTE: {
                while (current_char != '"') {
                    add();
                    count_col++;
                    current_char = gc();
                }
                count_col++;
                current_char = gc();
                if (current_char == ' ' || current_char == ',' || current_char == ')' || current_char == ';' ||
                    isalnum(current_char)) {
                    temp_index = TABLE_STRING.put(buf);
                    push_back(current_char);
                    clear_buf();
                    return Lexeme(LEX_STRING, temp_index);
                } else {
                    push_back(current_char);
                    std::cout << "ERROR WHILE READING STRING|readed:" << buf << std::endl;
                    throw current_char;
                }
            }
        }
    } while (true);
}
