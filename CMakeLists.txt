cmake_minimum_required(VERSION 3.29)
project(IU_Compilulki_Compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compilulki src/main.cpp
        src/settings/settings.cpp
        src/settings/settings.h
        src/lexical_analyzer/lexer.cpp
        src/lexical_analyzer/lexer.h
        src/lexical_analyzer/token.h
        src/lexical_analyzer/token.cpp
        src/lexical_analyzer/span.cpp
        src/lexical_analyzer/span.h
        src/lexical_analyzer/token_code.h
        src/syntax_analyzer/AST.h
        src/syntax_analyzer/Parser.h
        src/syntax_analyzer/Parser.cpp
        src/syntax_analyzer/AST.cpp
        src/syntax_analyzer/PrintVisitor.cpp
        src/syntax_analyzer/PrintVisitor.h
        src/utils/helper.cpp
        src/syntax_analyzer/SymbolTable.cpp
        src/syntax_analyzer/SymbolTableVisitor.cpp
)
