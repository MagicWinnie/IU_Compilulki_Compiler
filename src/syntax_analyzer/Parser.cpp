//
// Created by Nail Minnemullin on 08.10.2024.
//

#include "Parser.h"


Parser::Parser(std::vector<std::unique_ptr<Token>> tokens){
    this->tokens = std::move(tokens);
}

void Parser::parse() {
    try{
        auto program = parseProgram();
        std::cout<< "Parsed program" << std::endl;
    }
    catch (std::runtime_error &e){
        std::cerr << e.what() << std::endl;
    }

}

void Parser::expect(TokenCode code) {
    auto next_token = tokens[current_token]->get_code();
    if(next_token != code){
        throw std::runtime_error("Unexpected token: " + tokens[current_token]->to_string());
    }
    current_token++;
}

TokenCode Parser::peekNextToken() {
    return tokens[current_token]->get_code();
}

std::unique_ptr<Token> Parser::getNextToken() {
    return std::move(tokens[current_token++]);
}

void Parser::consumeToken() {
    current_token++;
}

std::unique_ptr<Entity> Parser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();
    program->programDeclaration = parseProgramDeclaration();
    program->classDeclarations = nullptr;  // No class declarations in this example
    return program;
}

std::unique_ptr<Entity> Parser::parseProgramDeclaration() {
    expect(PROGRAM);
    expect(COLON);

    auto node = std::make_unique<ProgramDeclarationNode>();
//    node->className = parseClassName();

    auto next_token = getNextToken();
    if(next_token->get_code() != IDENTIFIER){
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }
    else{
        node->className = (dynamic_cast<Identifier*>(next_token.get()))->get_identifier();
    }


    if (peekNextToken() == LEFT_PAREN) {
        consumeToken();
//        node->arguments = parseProgramArguments();
        node->arguments = nullptr;  // No arguments in this example
        //expect(RIGHT_PAREN);
    }
    return node;
}


