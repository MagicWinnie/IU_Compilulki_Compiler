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

std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    program->programDeclaration = parseProgramDeclaration();
    // TODO add class declarations parsing
    program->classDeclarations = std::vector<std::unique_ptr<Entity>>();  // No class declarations in this example
    return program;
}

std::unique_ptr<ProgramDeclaration> Parser::parseProgramDeclaration() {
    expect(PROGRAM);
    expect(COLON);

    auto node = std::make_unique<ProgramDeclaration>();
//    node->className = parseClassName();

    auto next_token = getNextToken();
    if(next_token->get_code() != IDENTIFIER){
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }

    node->className = (dynamic_cast<Identifier*>(next_token.get()))->get_identifier();


    if (peekNextToken() == LEFT_PAREN) {
        consumeToken();
//        node->arguments = parseProgramArguments();
        node->arguments = nullptr;  // No arguments in this example
        //expect(RIGHT_PAREN);
    }
    return node;
}

std::unique_ptr<Arguments> Parser::parseProgramArguments() {
    if (peekNextToken() == LEFT_PAREN) {
        consumeToken();
        auto expressions = parseExpressions();
        expect(RIGHT_PAREN);
        return std::make_unique<Arguments>(std::move(expressions));
    }
    return nullptr;  // Empty arguments
}



std::unique_ptr<Literals> Parser::parseLiterals() {
    std::vector<std::unique_ptr<Literal>> literals;
    literals.push_back(parseLiteral());
    while (peekNextToken() == COMMA) {
        consumeToken();
        literals.push_back(parseLiteral());
    }
    return std::make_unique<Literals>(std::move(literals));
}

std::unique_ptr<Literal> Parser::parseLiteral() {
    auto next_token = getNextToken();
    switch (next_token->get_code()) {
    case INTEGER:
        return std::make_unique<Literal>(Literal::Type::INTEGER, next_token->to_string());
    case REAL:
        return std::make_unique<Literal>(Literal::Type::REAL, next_token->to_string());
    case BOOLEAN:
        return std::make_unique<Literal>(Literal::Type::BOOLEAN, next_token->to_string());
    default:
        throw std::runtime_error("Expected literal, got: " + tokens[current_token]->to_string());
    }
}

std::unique_ptr<Arguments> Parser::parseArguments() {
    if (peekNextToken() == LEFT_PAREN) {
        consumeToken();
        auto expressions = parseExpressions();
        expect(RIGHT_PAREN);
        return std::make_unique<Arguments>(std::move(expressions));
    }
    return nullptr;  // Empty arguments
}


std::unique_ptr<Expression> Parser::parseExpression() {
    if (isPrimary(peekNextToken())) {
        // return std::make_unique<Expression>(parsePrimary());
        return nullptr;
    } else {
        // return std::make_unique<Expression>(parseCompoundExpression());
        return nullptr;
    }
}



std::vector<std::unique_ptr<ClassDeclaration>> Parser::parseClassDeclarations() {
    std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;
    classDeclarations.push_back(parseClassDeclaration());
    while (peekNextToken() == CLASS) {
        classDeclarations.push_back(parseClassDeclaration());
    }
    return classDeclarations;
}

std::unique_ptr<ClassDeclaration> Parser::parseClassDeclaration() {
    auto classDeclaration = std::make_unique<ClassDeclaration>();
    expect(CLASS);
    classDeclaration->className = parseClassName();
    classDeclaration->extension = parseExtension();
    expect(IS);
    classDeclaration->classBody = parseClassBody();
    expect(END);
    return std::make_unique<ClassDeclaration>(std::move(className), std::move(extension), std::move(classBody));
}

std::unique_ptr<ClassBody> Parser::parseClassBody() {
    auto memberDeclarations = parseMemberDeclarations();
    return std::make_unique<ClassBody>(memberDeclarations);
}

std::unique_ptr<ClassName> Parser::parseClassName() {
    auto next_token = getNextToken();
    if(next_token->get_code() != IDENTIFIER){
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }
    return std::make_unique<ClassName>((dynamic_cast<Identifier*>(next_token.get()))->get_identifier());
}



std::unique_ptr<Extension> Parser::parseExtension() {
    if (peekNextToken() == EXTENDS) {
        consumeToken();
        auto className = parseClassName();
        return std::make_unique<Extension>(className);
    }
    return nullptr;  // Empty extension
}

std::unique_ptr<Body> Parser::parseBody() {
    auto bodyDeclarations = parseBodyDeclarations();
    return std::make_unique<Body>(bodyDeclarations);
}

std::unique_ptr<BodyDeclarations> Parser::parseBodyDeclarations() {
    std::vector<std::unique_ptr<BodyDeclaration>> bodyDeclarations;
    do {
        bodyDeclarations.push_back(parseBodyDeclaration());
        // TODO add statement parsing
    } while (peekNextToken() == VAR );
    return std::make_unique<BodyDeclarations>(bodyDeclarations);
}

std::unique_ptr<BodyDeclaration> Parser::parseBodyDeclaration() {
    if (peekNextToken() == VAR) {
        return std::make_unique<BodyDeclaration>(parseVariableDeclaration());
    }
    // TODO add statement parsing
    // else if (peekNextToken() == ???) {
    //     return std::make_unique<BodyDeclaration>(parseStatement());
    // }
    throw std::runtime_error("Unexpected token in body declaration: " + tokens[current_token]->to_string());
}

std::unique_ptr<Statement> Parser::parseStatement() {
    // Implement the logic to parse the statement

}

std::unique_ptr<Assignment> Parser::parseAssignment() {
    auto assignment = std::make_unique<Assignment>();
    assignment->variableName = parseVariableName();
    expect(COLON_EQUAL);
    assignment->expression = parseExpression();
}





std::vector<std::unique_ptr<MemberDeclaration>> Parser::parseMemberDeclarations() {
    std::vector<std::unique_ptr<MemberDeclaration>> memberDeclarations;
    memberDeclarations.push_back(parseMemberDeclaration());
    // TODO add constructor declaration parsing
    while (peekNextToken() == VAR || peekNextToken() == METHOD) {
        memberDeclarations.push_back(parseMemberDeclaration());
    }
    return memberDeclarations;
}
std::unique_ptr<MemberDeclaration> Parser::parseMemberDeclaration() {
    auto nextToken = peekNextToken();
    auto memberDeclaration = std::make_unique<MemberDeclaration>();
    if (nextToken == VAR) {
        memberDeclaration->variableDeclaration = parseVariableDeclaration();
        return memberDeclaration;
    } else if (nextToken == METHOD)
    {
        memberDeclaration->methodDeclaration = parseMethodDeclaration();
        return memberDeclaration;
    }
    // TODO add constructor declaration parsing
    // } else if (nextToken == ) {
    //     return std::make_unique<MemberDeclaration>(MemberDeclaration::Type::CONSTRUCTOR, parseConstructorDeclaration());
    // }
    throw std::runtime_error("Unexpected token in member declaration: " + tokens[current_token]->to_string());
}

std::unique_ptr<VariableDeclaration> Parser::parseVariableDeclaration() {
    expect(VAR);
    auto variableName = parseVariableName();
    expect(COLON);
    auto expression = parseExpression();
    return std::make_unique<VariableDeclaration>(std::move(variableName), std::move(expression));
}

std::unique_ptr<MethodDeclaration> Parser::parseMethodDeclaration() {
    expect(METHOD);
    auto methodName = parseMethodName();
    expect(LEFT_PAREN);
    auto parameters = parseParameters();
    expect(RIGHT_PAREN);
    auto returnType = parseReturnType();
    expect(IS);
    auto body = parseBody();
    expect(END);
    return std::make_unique<MethodDeclaration>(std::move(methodName), std::move(parameters), std::move(returnType), std::move(body));
}

std::unique_ptr<MethodName> Parser::parseMethodName() {
    auto next_token = getNextToken();
    if(next_token->get_code() != IDENTIFIER){
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }
    return std::make_unique<MethodName>((dynamic_cast<Identifier*>(next_token.get()))->get_identifier());
}

std::unique_ptr<Parameters> Parser::parseParameters() {
    auto parameters = std::make_unique<Parameters>();
    if (peekNextToken() != RIGHT_PAREN) {  // Assuming RIGHT_PAREN is the token that closes the parameter list
        parameters->parameters.push_back(parseParameter());
        while (peekNextToken() == COMMA) {
            consumeToken();
            parameters->parameters.push_back(parseParameter());
        }
    }
    return parameters;
}

std::unique_ptr<Parameter> Parser::parseParameter() {
    auto identifier = getNextToken();
    if (identifier->get_code() != IDENTIFIER) {
        throw std::runtime_error("Expected parameter identifier, got: " + identifier->to_string());
    }
    expect(COLON);
    auto className = parseClassName();
    return std::make_unique<Parameter>(identifier->to_string(), std::move(className));
}

std::unique_ptr<ReturnType> Parser::parseReturnType() {
    // Implement the logic to parse the return type
    auto className = parseClassName();
    return std::make_unique<ReturnType>(std::move(className));
}



std::unique_ptr<VariableName> Parser::parseVariableName() {
    auto next_token = getNextToken();
    if(next_token->get_code() != IDENTIFIER){
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }
    return std::make_unique<VariableName>((dynamic_cast<Identifier*>(next_token.get()))->get_identifier());
}




bool Parser::isPrimary(TokenCode code) {
    return code == INTEGER || code == REAL || code == BOOLEAN || code == THIS || code == IDENTIFIER;
}