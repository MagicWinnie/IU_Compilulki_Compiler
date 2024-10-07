//
// Created by Nail Minnemullin on 08.10.2024.
//

#ifndef IU_COMPILULKI_COMPILER_PARSER_H
#define IU_COMPILULKI_COMPILER_PARSER_H


#include "vector"
#include "iostream"
#include "memory"
#include "../lexical_analyzer/Token.h"
#include "Nodes.h"

class Parser {
    std::vector<std::unique_ptr<Token>> tokens;
    int current_token = 0;

public:
    explicit Parser(std::vector<std::unique_ptr<Token>> tokens);

    void parse();

    void expect(TokenCode code);

    TokenCode peekNextToken();

    std::unique_ptr<Token> getNextToken();

    void consumeToken();

    std::unique_ptr<Entity> parseProgram();

    std::unique_ptr<Entity> parseProgramDeclaration();

};


#endif //IU_COMPILULKI_COMPILER_PARSER_H
