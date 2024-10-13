//
// Created by Nail Minnemullin on 08.10.2024.
//

#include "Parser.h"

#include "PrintVisitor.h"


Parser::Parser(std::vector<std::unique_ptr<Token>> tokens)
{
    this->tokens = std::move(tokens);
}

void Parser::parse()
{
    try
    {
        auto program = parseProgram();
        std::cout << "Parsed program" << std::endl;
        PrintVisitor printer;
        program->accept(printer);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void Parser::expectAndConsume(TokenCode code)
{
    auto next_token = tokens[current_token]->get_code();
    if (next_token != code)
    {
        throw std::runtime_error("Unexpected token: " + tokens[current_token]->to_string());
    }
    current_token++;
}

void Parser::expect(TokenCode code)
{
    auto next_token = tokens[current_token]->get_code();
    if (next_token != code)
    {
        throw std::runtime_error("Unexpected token: " + tokens[current_token]->to_string());
    }
}

TokenCode Parser::peekNextToken(int offset)
{
    if (current_token >= tokens.size())
    {
        return END_OF_FILE;
    }
    return tokens[current_token + offset]->get_code();
}

std::unique_ptr<Token> Parser::getNextToken()
{
    return std::move(tokens[current_token++]);
}

void Parser::consumeToken()
{
    current_token++;
}

std::unique_ptr<Program> Parser::parseProgram()
{
    auto program = std::make_unique<Program>();
    program->programDeclaration = parseProgramDeclaration();
    // TODO add class declarations parsing
    program->classDeclarations = parseClassDeclarations();
    return program;
}

std::unique_ptr<ProgramDeclaration> Parser::parseProgramDeclaration()
{
    expectAndConsume(PROGRAM);
    expectAndConsume(COLON);

    auto node = std::make_unique<ProgramDeclaration>();
    //    node->className = parseClassName();

    expect(IDENTIFIER);


    node->className = std::make_unique<ClassName>((dynamic_cast<Identifier*>(getNextToken().get()))->get_identifier());

    node->arguments = parseProgramArguments();
    return node;
}

std::unique_ptr<ProgramArguments> Parser::parseProgramArguments()
{
    if (peekNextToken() == LEFT_PAREN)
    {
        consumeToken(); // consume LEFT_PAREN
        if (peekNextToken() == RIGHT_PAREN)
        {
            consumeToken(); // consume RIGHT_PAREN
            return nullptr; // Empty arguments
        }

        auto literals = parseLiterals();
        expectAndConsume(RIGHT_PAREN);
        return std::make_unique<ProgramArguments>(std::move(literals));
    }
    return nullptr; // Empty arguments
}


std::unique_ptr<Literals> Parser::parseLiterals()
{
    std::vector<std::unique_ptr<Literal>> literals;
    literals.push_back(parseLiteral());
    while (peekNextToken() == COMMA)
    {
        consumeToken();
        literals.push_back(parseLiteral());
    }
    return std::make_unique<Literals>(std::move(literals));
}

std::unique_ptr<Literal> Parser::parseLiteral()
{
    auto next_token = getNextToken();
    switch (next_token->get_code())
    {
    // TODO fix
    case INTEGER:
        return std::make_unique<Literal>(next_token->to_string());
    case REAL:
        return std::make_unique<Literal>(next_token->to_string());
    case BOOLEAN:
        return std::make_unique<Literal>(next_token->to_string());
    default:
        throw std::runtime_error("Expected literal, got: " + tokens[current_token]->to_string());
    }
}

std::unique_ptr<Arguments> Parser::parseArguments()
{
    if (peekNextToken() == RIGHT_PAREN)
    {
        return nullptr;
    }

    auto expressions = parseExpressions();
    return std::make_unique<Arguments>(std::move(expressions));
}

std::unique_ptr<Expressions> Parser::parseExpressions()
{
    auto expressions = std::make_unique<Expressions>();
    expressions->expressions.push_back(parseExpression());
    while (peekNextToken() == COMMA)
    {
        consumeToken();
        expressions->expressions.push_back(parseExpression());
    }
    return expressions;
}


std::unique_ptr<Expression> Parser::parseExpression()
{
    if (peekNextToken() == THIS)
    {
        if (peekNextToken(1) == DOT)
        {
            auto expression = std::make_unique<Expression>();
            expression->compoundExpression = parseCompoundExpression();
            return expression;
        }
        auto expression = std::make_unique<Expression>();
        expression->primary = parsePrimary();
        return expression;
    }
    if (isPrimary(peekNextToken()))
    {
        auto expression = std::make_unique<Expression>();
        expression->primary = parsePrimary();
        return expression;
    }
    else
    {
        auto expression = std::make_unique<Expression>();
        expression->compoundExpression = parseCompoundExpression();
        return expression;
    }
}

std::unique_ptr<Primary> Parser::parsePrimary()
{
    auto token = getNextToken();
    switch (token->get_code())
    {
    case INTEGER:
        return std::make_unique<Primary>(token->to_string());
    case REAL:
        return std::make_unique<Primary>(token->to_string());
    case BOOLEAN:
        return std::make_unique<Primary>(token->to_string());
    case THIS:
        return std::make_unique<Primary>(token->to_string());
    default:
        throw std::runtime_error("Unexpected token in primary: " + token->to_string());
    }
}

// TODO needs to be testes
std::unique_ptr<CompoundExpression> Parser::parseCompoundExpression()
{
    auto identifier = getNextToken();
    std::vector<std::unique_ptr<CompoundExpression>> compoundExpressions;

    if (identifier->get_code() == THIS)
    {
        if (peekNextToken() == DOT)
        {
            consumeToken(); // consume DOT
            compoundExpressions.push_back(parseCompoundExpression());
            return std::make_unique<CompoundExpression>("this", nullptr, std::move(compoundExpressions));
        }
        else
        {
            throw std::runtime_error("Expected '.' after 'this', got: " + peekNextToken());
        }
    }
    if (identifier->get_code() != IDENTIFIER)
    {
        throw std::runtime_error("Expected identifier, got: " + identifier->to_string());
    }

    // Check if it's an array type with square brackets
    if (peekNextToken() == LEFT_SQUARE_BRACKET)
    {
        consumeToken(); // consume LEFT_SQUARE_BRACKET
        compoundExpressions.push_back(parseCompoundExpression()); // parse the expression inside the square brackets
        expectAndConsume(RIGHT_SQUARE_BRACKET); // consume the closing bracket

        // Now check if it’s followed by a function call (parentheses)
        if (peekNextToken() == LEFT_PAREN)
        {
            consumeToken(); // consume LEFT_PAREN
            auto arguments = parseArguments(); // parse the arguments inside the parentheses
            expectAndConsume(RIGHT_PAREN); // consume the closing parenthesis

            if (peekNextToken() == DOT)
            {
                consumeToken(); // consume DOT
                compoundExpressions.push_back(parseCompoundExpression());
            }

            return std::make_unique<CompoundExpression>(identifier->to_string(), std::move(arguments),
                                                        std::move(compoundExpressions));
        }


        return std::make_unique<CompoundExpression>(identifier->to_string(), nullptr, std::move(compoundExpressions));
    }

    if (peekNextToken() == LEFT_PAREN)
    {
        consumeToken(); // consume LEFT_PAREN
        auto arguments = parseArguments();
        expectAndConsume(RIGHT_PAREN);

        if (peekNextToken() == DOT)
        {
            consumeToken(); // consume DOT
            compoundExpressions.push_back(parseCompoundExpression());
            return std::make_unique<CompoundExpression>(identifier->to_string(), std::move(arguments),
                                                        std::move(compoundExpressions));
        }

        return std::make_unique<CompoundExpression>(identifier->to_string(), std::move(arguments));
    }

    if (peekNextToken() == DOT)
    {
        consumeToken(); // consume DOT
        compoundExpressions.push_back(parseCompoundExpression());
        return std::make_unique<CompoundExpression>(identifier->to_string(), nullptr, std::move(compoundExpressions));
    }


    return std::make_unique<CompoundExpression>(identifier->to_string());
}


std::unique_ptr<ClassDeclarations> Parser::parseClassDeclarations()
{
    auto classDeclarations = std::make_unique<ClassDeclarations>();
    classDeclarations->classDeclarations.push_back(parseClassDeclaration());
    while (peekNextToken() == CLASS)
    {
        classDeclarations->classDeclarations.push_back(parseClassDeclaration());
    }
    return classDeclarations;
}

std::unique_ptr<ClassDeclaration> Parser::parseClassDeclaration()
{
    auto classDeclaration = std::make_unique<ClassDeclaration>();
    expectAndConsume(CLASS);
    classDeclaration->className = parseClassName();
    classDeclaration->extension = parseExtension();
    expectAndConsume(IS);
    classDeclaration->classBody = parseClassBody();
    expectAndConsume(END);
    return classDeclaration;
}

std::unique_ptr<ClassBody> Parser::parseClassBody()
{
    return std::make_unique<ClassBody>(parseMemberDeclarations());
}

std::unique_ptr<ClassName> Parser::parseClassName()
{
    const auto next_token = getNextToken();
    if (next_token->get_code() != IDENTIFIER)
    {
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }

    std::unique_ptr<ClassName> subClassName = nullptr;
    if (peekNextToken() == LEFT_SQUARE_BRACKET)
    {
        consumeToken();
        subClassName = parseClassName();
        expectAndConsume(RIGHT_SQUARE_BRACKET);
    }

    return std::make_unique<ClassName>((dynamic_cast<Identifier*>(next_token.get()))->get_identifier(),
                                       std::move(subClassName));
}


std::unique_ptr<Extension> Parser::parseExtension()
{
    if (peekNextToken() == EXTENDS)
    {
        consumeToken();
        auto className = parseClassName();
        return std::make_unique<Extension>(std::move(className));
    }
    return nullptr; // Empty extension
}

std::unique_ptr<Body> Parser::parseBody()
{
    return std::make_unique<Body>(parseBodyDeclarations());
}

std::unique_ptr<BodyDeclarations> Parser::parseBodyDeclarations()
{
    std::vector<std::unique_ptr<BodyDeclaration>> bodyDeclarations;
    do
    {
        bodyDeclarations.push_back(parseBodyDeclaration());
        // TODO add statement parsing
    }
    while (peekNextToken() == VAR || peekNextToken() == IF || peekNextToken() == WHILE || peekNextToken() == RETURN ||
        peekNextToken() == IDENTIFIER ||
        peekNextToken() == THIS);
    return std::make_unique<BodyDeclarations>(std::move(bodyDeclarations));
}

std::unique_ptr<BodyDeclaration> Parser::parseBodyDeclaration()
{
    if (peekNextToken() == END)
    {
        return nullptr;
    }

    if (peekNextToken() == VAR && peekNextToken(2) != COLON_EQUAL)
    {
        return std::make_unique<BodyDeclaration>(parseVariableDeclaration());
    }
    else
    {
        return std::make_unique<BodyDeclaration>(parseStatement());
    }
    // TODO add statement parsing
    // else if (peekNextToken() == ???) {
    //     return std::make_unique<BodyDeclaration>(parseStatement());
    // }
    throw std::runtime_error("Unexpected token in body declaration: " + tokens[current_token]->to_string());
}

std::unique_ptr<Statement> Parser::parseStatement()
{
    // Implement the logic to parse the statement
    if (peekNextToken(1) == COLON_EQUAL)
    {
        auto statement = std::make_unique<Statement>();
        statement->assignment = parseAssignment();
        return statement;
    }
    if (peekNextToken() == IF)
    {
        auto statement = std::make_unique<Statement>();
        statement->ifStatement = parseIfStatement();
        return statement;
    }
    if (peekNextToken() == WHILE)
    {
        auto statement = std::make_unique<Statement>();
        statement->whileLoop = parseWhileLoop();
        return statement;
    }
    if (peekNextToken() == IDENTIFIER)
    {
        auto statement = std::make_unique<Statement>();
        statement->expression = parseExpression();
        return statement;
    }
    if (peekNextToken() == RETURN)
    {
        //TODO add return
        auto statement = std::make_unique<Statement>();
        statement->returnStatement = parseReturnStatement();
        return statement;
    }
    if (peekNextToken() == THIS)
    {
        auto statement = std::make_unique<Statement>();
        statement->expression = parseExpression();
        return statement;
    }
    else
    {
        throw std::runtime_error("Unexpected token in statement: " + tokens[current_token]->to_string());
    }
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement()
{
    expectAndConsume(RETURN);
    auto expression = parseExpression();
    return std::make_unique<ReturnStatement>(std::move(expression));
}

std::unique_ptr<IfStatement> Parser::parseIfStatement()
{
    expectAndConsume(IF);
    auto expression = parseExpression();
    auto ifBranch = parseIfBranch();
    auto elseBranch = parseElsebranch();
    expectAndConsume(END);
    return std::make_unique<IfStatement>(std::move(expression), std::move(ifBranch), std::move(elseBranch));
}

std::unique_ptr<IfBranch> Parser::parseIfBranch()
{
    expectAndConsume(THEN);
    return std::make_unique<IfBranch>(parseBody());
}

std::unique_ptr<ElseBranch> Parser::parseElsebranch()
{
    if (peekNextToken() == ELSE)
    {
        consumeToken();
        return std::make_unique<ElseBranch>(parseBody());
    }
    return nullptr; // Empty else branch
}

std::unique_ptr<WhileLoop> Parser::parseWhileLoop()
{
    expectAndConsume(WHILE);
    auto expression = parseExpression();
    expectAndConsume(LOOP);
    auto body = parseBody();
    expectAndConsume(END);
    return std::make_unique<WhileLoop>(std::move(expression), std::move(body));
}

std::unique_ptr<Assignment> Parser::parseAssignment()
{
    auto assignment = std::make_unique<Assignment>();
    assignment->variableName = parseVariableName();
    expectAndConsume(COLON_EQUAL);
    assignment->expression = parseExpression();
    return assignment;
}


std::unique_ptr<MemberDeclarations> Parser::parseMemberDeclarations()
{
    auto memberDeclarations = std::make_unique<MemberDeclarations>();
    auto memberDeclaration = parseMemberDeclaration();
    memberDeclarations->member_declarations.push_back(std::move(memberDeclaration));
    // TODO add constructor declaration parsing
    while (peekNextToken() == VAR || peekNextToken() == METHOD || peekNextToken() == THIS)
    {
        memberDeclarations->member_declarations.push_back(parseMemberDeclaration());
    }
    return memberDeclarations;
}

std::unique_ptr<MemberDeclaration> Parser::parseMemberDeclaration()
{
    auto nextToken = peekNextToken();
    auto memberDeclaration = std::make_unique<MemberDeclaration>();
    if (nextToken == VAR)
    {
        memberDeclaration->variableDeclaration = parseVariableDeclaration();
        return memberDeclaration;
    }
    else if (nextToken == METHOD)
    {
        memberDeclaration->methodDeclaration = parseMethodDeclaration();
        return memberDeclaration;
        // TODO maybe error??
    }
    else if (nextToken == THIS)
    {
        memberDeclaration->constructorDeclaration = parseConstructorDeclaration();
        return memberDeclaration;
    }
    throw std::runtime_error("Unexpected token in member declaration: " + tokens[current_token]->to_string());
}

std::unique_ptr<ConstructorDeclaration> Parser::parseConstructorDeclaration()
{
    expectAndConsume(THIS);
    expectAndConsume(LEFT_PAREN);
    auto parameters = parseParameters();
    expectAndConsume(RIGHT_PAREN);
    expectAndConsume(IS);
    auto body = parseBody();
    expectAndConsume(END);
    return std::make_unique<ConstructorDeclaration>(std::move(parameters), std::move(body));
}

std::unique_ptr<VariableDeclaration> Parser::parseVariableDeclaration()
{
    expectAndConsume(VAR);
    auto variableName = parseVariableName();

    expectAndConsume(COLON);
    auto expression = parseExpression();
    return std::make_unique<VariableDeclaration>(std::move(variableName), std::move(expression));
}

std::unique_ptr<MethodDeclaration> Parser::parseMethodDeclaration()
{
    expectAndConsume(METHOD);
    auto methodName = parseMethodName();
    if (methodName->name == "mergeSort")
    {
        int a = 0;
    }
    expectAndConsume(LEFT_PAREN);
    auto parameters = parseParameters();
    expectAndConsume(RIGHT_PAREN);
    std::unique_ptr<ReturnType> returnType = nullptr;
    if (peekNextToken() == COLON)
    {
        consumeToken();
        returnType = parseReturnType();
    }

    expectAndConsume(IS);
    auto body = parseBody();
    expectAndConsume(END);
    return std::make_unique<MethodDeclaration>(methodName, parameters, returnType, body);
}

std::unique_ptr<MethodName> Parser::parseMethodName()
{
    auto next_token = getNextToken();
    if (next_token->get_code() != IDENTIFIER)
    {
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }
    return std::make_unique<MethodName>((dynamic_cast<Identifier*>(next_token.get()))->get_identifier());
}

std::unique_ptr<Parameters> Parser::parseParameters()
{
    auto next_token = peekNextToken();
    // No parameters
    if (next_token == RIGHT_PAREN)
    {
        return nullptr;
    }
    else
    {
        auto parameters = std::make_unique<Parameters>();
        parameters->parameters.push_back(parseParameter());
        while (peekNextToken() == COMMA)
        {
            consumeToken();
            parameters->parameters.push_back(parseParameter());
        }
        return parameters;
    }


    return nullptr;


    // TODO implement the logic to parse the parameters
    // if (peekNextToken() != RIGHT_PAREN) {  // Assuming RIGHT_PAREN is the token that closes the parameter list
    //     parameters->parameters.push_back(parseParameter());
    //     while (peekNextToken() == COMMA) {
    //         consumeToken();
    //         parameters->parameters.push_back(parseParameter());
    //     }
    // }
}

std::unique_ptr<Parameter> Parser::parseParameter()
{
    expect(IDENTIFIER);
    auto identifier = getNextToken();
    expectAndConsume(COLON);
    auto className = parseClassName();
    return std::make_unique<Parameter>(identifier->to_string(), std::move(className));
}

std::unique_ptr<ReturnType> Parser::parseReturnType()
{
    // Implement the logic to parse the return type
    auto className = parseClassName();
    return std::make_unique<ReturnType>(std::move(className));
}


std::unique_ptr<VariableName> Parser::parseVariableName()
{
    auto next_token = getNextToken();
    if (next_token->get_code() != IDENTIFIER)
    {
        throw std::runtime_error("Expected identifier, got: " + tokens[current_token]->to_string());
    }
    return std::make_unique<VariableName>((dynamic_cast<Identifier*>(next_token.get()))->get_identifier());
}


bool Parser::isPrimary(TokenCode code)
{
    return code == INTEGER || code == REAL || code == BOOLEAN;
}
