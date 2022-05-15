#ifndef TABLE_STRING_H
#define TABLE_STRING_H


#include <string>
#include <cstring>

class Table_string {
    std::string *pointer;
    int size;
    int top;
public:
    Table_string(int max_size);

    ~Table_string();

    std::string &operator[](int k);

    int put(const char *buf);

    int put(const std::string &s);
};

#endif //TABLE_STRING_H
