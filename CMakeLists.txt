cmake_minimum_required(VERSION 3.16)
project(Interpretator)

set(CMAKE_CXX_STANDARD 14)

add_executable(interpretator
        main.cpp
        Interpretator.cpp
        Poliz.cpp
        Executor.cpp
        Parser.cpp
        Scanner.cpp
        Identifier.cpp
        Lexeme.cpp
        Table_identifier.cpp
        Table_label.cpp
        Table_string.cpp
        Type_of_lexeme.cpp
        Label.cpp
        )