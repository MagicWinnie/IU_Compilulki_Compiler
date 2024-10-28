//
// Created by Nail Minnemullin on 08.10.2024.
//

#include "Parser.h"

#include <iostream>
#include <fstream>

#include "../utils/helper.cpp"
#include "PrintVisitor.h"


Parser::Parser(std::vector<std::unique_ptr<Token>> tokens, const std::string& infile_path, const bool& debug)
{
    this->tokens = std::move(tokens);
    this->infile_path = infile_path;
    this->debug = debug;
}

std::unique_ptr<Program> Parser::parse()
{
    try
    {
        auto program = parseProgram();
        if (this->debug)
        {
            std::filesystem::path path(this->infile_path);
            path.replace_extension(".syntax");
            std::cout << "Syntax analyzer writing to " << path << std::endl;

            std::ofstream output_file;
            output_file.open(path);

            PrintVisitor printer(output_file);
            program->accept(printer);

            output_file.close();
        }
        return program;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void Parser::expectAndConsume(const TokenCode code)
{
    expect(code);
    current_token++;
}


void throwError(const std::string& expected, const std::string& got, const int lineNumber, const int column)
{
    const std::string errorString = "Expected token: " + expected + " at line: " + std::to_string(lineNumber) +
        " column: " + std::to_string(column) + ", got: " + got;
    throw std::runtime_error(errorString);
}

void Parser::expect(const TokenCode code) const
{
    const auto next_token = tokens[current_token]->get_code();
    if (next_token != code)
    {
        const auto span = tokens[current_token]->get_span();
        const auto line = span.get_line_num();
        const auto column = span.get_pos_begin();
        throwError(getEnumName(code), getEnumName(tokens[current_token]->get_code()), line, column);
    }
}

TokenCode Parser::peekNextToken(const int offset) const
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
    program->classDeclarations = parseClassDeclarations();
    return program;
}

std::unique_ptr<ProgramDeclaration> Parser::parseProgramDeclaration()
{
    expectAndConsume(PROGRAM);
    expectAndConsume(COLON);

    auto node = std::make_unique<ProgramDeclaration>();

    expect(IDENTIFIER);

    node->className = std::make_unique<ClassName>((dynamic_cast<Identifier*>(getNextToken().get()))->get_identifier(),
                                                  tokens[current_token]->get_span());
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
    const auto next_token = getNextToken();
    switch (next_token->get_code())
    {
    // TODO fix
    case INTEGER:
        {
            auto integer = dynamic_cast<Integer*>(next_token.get());
            std::unique_ptr<Literal> literal = std::make_unique<IntLiteral>(integer->value);
            return literal;
        }

    case REAL:
        {
            auto real = dynamic_cast<Real*>(next_token.get());
            std::unique_ptr<Literal> literal = std::make_unique<RealLiteral>(real->value);
            return literal;
        }

    case BOOLEAN:
        {
            auto boolean = dynamic_cast<Boolean*>(next_token.get());
            std::unique_ptr<Literal> literal = std::make_unique<BoolLiteral>(boolean->value);
            return literal;
        }

    default:
        const auto span = tokens[current_token]->get_span();
        throwError("literal", getEnumName(tokens[current_token]->get_code()),
                   span.get_line_num(), span.get_pos_begin());
    }
    return nullptr;
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
    const auto token = getNextToken();
    switch (token->get_code())
    {
    case TokenCode::INTEGER: {
            auto integer = dynamic_cast<Integer*>(token.get());
            std::unique_ptr<Literal> literal = std::make_unique<IntLiteral>(integer->value);
            return std::make_unique<Primary>(literal);
    }
    case TokenCode::REAL: {
            auto real = dynamic_cast<Real*>(token.get());
             std::unique_ptr<Literal> literal = std::make_unique<RealLiteral>(real->value);
            return std::make_unique<Primary>(literal);
    }
    case TokenCode::BOOLEAN: {
            auto boolean = dynamic_cast<Boolean*>(token.get());
             std::unique_ptr<Literal> literal = std::make_unique<BoolLiteral>(boolean->value);
            return std::make_unique<Primary>(literal);
    }
    case TokenCode::THIS: {
            auto className = std::make_unique<ClassName>(ClassName(token->to_string(), token->get_span()));
            return std::make_unique<Primary>(className);
    }
    default:
        throw std::runtime_error("Unexpected token: " + token->to_string());
    }
}

// TODO needs to be testes
std::unique_ptr<CompoundExpression> Parser::parseCompoundExpression()
{
    const auto identifier = getNextToken();
    std::vector<std::unique_ptr<CompoundExpression>> compoundExpressions;

    const auto span = tokens[current_token]->get_span();

    if (identifier->get_code() == THIS)
    {
        if (peekNextToken() == DOT)
        {
            consumeToken(); // consume DOT
            compoundExpressions.push_back(parseCompoundExpression());
            return std::make_unique<CompoundExpression>("this", span, nullptr, std::move(compoundExpressions));
        }
        throw std::runtime_error("Expected '.' after 'this', got: " + peekNextToken());
    }
    if (identifier->get_code() != IDENTIFIER)
    {
        throwError("identifier", getEnumName(tokens[current_token]->get_code()),
                   span.get_line_num(), span.get_pos_begin());
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

            return std::make_unique<CompoundExpression>(identifier->to_string(), span, std::move(arguments),
                                                        std::move(compoundExpressions));
        }


        return std::make_unique<CompoundExpression>(identifier->to_string(), span, nullptr,
                                                    std::move(compoundExpressions));
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
            return std::make_unique<CompoundExpression>(identifier->to_string(), span, std::move(arguments),
                                                        std::move(compoundExpressions));
        }

        return std::make_unique<CompoundExpression>(identifier->to_string(), span, std::move(arguments));
    }

    if (peekNextToken() == DOT)
    {
        consumeToken(); // consume DOT
        compoundExpressions.push_back(parseCompoundExpression());
        return std::make_unique<CompoundExpression>(identifier->to_string(), span, nullptr,
                                                    std::move(compoundExpressions));
    }


    return std::make_unique<CompoundExpression>(identifier->to_string(), span);
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
    const auto span = tokens[current_token]->get_span();
    if (next_token->get_code() != IDENTIFIER)
    {
        throwError("identifier", getEnumName(tokens[current_token]->get_code()), span.get_line_num(),
                   span.get_pos_begin());
    }

    std::unique_ptr<ClassName> subClassName = nullptr;
    if (peekNextToken() == LEFT_SQUARE_BRACKET)
    {
        consumeToken();
        subClassName = parseClassName();
        expectAndConsume(RIGHT_SQUARE_BRACKET);
    }

    return std::make_unique<ClassName>((dynamic_cast<Identifier*>(next_token.get()))->get_identifier(), span,
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
    std::vector<std::unique_ptr<BodyDeclaration>> bodyDeclarationsVector;
    do
    {
        auto bodyDeclaration = parseBodyDeclaration();
        if(bodyDeclaration && bodyDeclaration->variableDeclaration) bodyDeclaration->variableDeclaration->bodyParent = bodyDeclaration.get();
        if(bodyDeclaration && bodyDeclaration->statement) bodyDeclaration->statement->parent = bodyDeclaration.get();
        bodyDeclarationsVector.push_back(std::move(bodyDeclaration));
    }
    while (peekNextToken() == VAR || peekNextToken() == IF || peekNextToken() == WHILE || peekNextToken() == RETURN ||
        peekNextToken() == IDENTIFIER ||
        peekNextToken() == THIS);
    auto bodyDeclarations =  std::make_unique<BodyDeclarations>(std::move(bodyDeclarationsVector));
    for(auto &bodyDeclaration : bodyDeclarations->bodyDeclarations)
    {
        if(bodyDeclaration)  bodyDeclaration->parent = bodyDeclarations.get();
    }
    return bodyDeclarations;
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
    return std::make_unique<BodyDeclaration>(parseStatement());
}

std::unique_ptr<Statement> Parser::parseStatement()
{
    // Implement the logic to parse the statement
    if (peekNextToken(1) == COLON_EQUAL)
    {
        std::unique_ptr<Statement> statement = parseAssignment();
        return statement;
    }
    if (peekNextToken() == IF)
    {
        std::unique_ptr<Statement> statement = parseIfStatement();
        return statement;
    }
    if (peekNextToken() == WHILE)
    {
         std::unique_ptr<Statement> statement = parseWhileLoop();
        return statement;
    }
    if (peekNextToken() == IDENTIFIER)
    {
         std::unique_ptr<Statement> statement = parseExpression();
        return statement;
    }
    if (peekNextToken() == RETURN)
    {
        //TODO add return
        std::unique_ptr<Statement> statement =  parseReturnStatement();

        return statement;
    }
    if (peekNextToken() == THIS)
    {
        std::unique_ptr<Statement> statement = parseExpression();
        return statement;
    }
    const auto span = tokens[current_token]->get_span();
    throwError(":=, if, while, identifier, return or this", getEnumName(tokens[current_token]->get_code()),
               span.get_line_num(), span.get_pos_begin());
    return nullptr;
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
    auto elseBranch = parseElseBranch();
    expectAndConsume(END);
    return std::make_unique<IfStatement>(std::move(expression), std::move(ifBranch), std::move(elseBranch));
}

std::unique_ptr<IfBranch> Parser::parseIfBranch()
{
    expectAndConsume(THEN);
    return std::make_unique<IfBranch>(parseBody());
}

std::unique_ptr<ElseBranch> Parser::parseElseBranch()
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
    const auto nextToken = peekNextToken();
    auto memberDeclaration = std::make_unique<MemberDeclaration>();
    if (nextToken == VAR)
    {
        memberDeclaration->variableDeclaration = parseVariableDeclaration();
        memberDeclaration->variableDeclaration->memberParent = memberDeclaration.get();
        return memberDeclaration;
    }
    if (nextToken == METHOD)
    {
        memberDeclaration->methodDeclaration = parseMethodDeclaration();
        return memberDeclaration;
        // TODO maybe error??
    }
    if (nextToken == THIS)
    {
        memberDeclaration->constructorDeclaration = parseConstructorDeclaration();
        return memberDeclaration;
    }
    const auto span = tokens[current_token]->get_span();
    throwError("var, method, or this", getEnumName(tokens[current_token]->get_code()),
               span.get_line_num(), span.get_pos_begin());
    return nullptr;
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
    return std::make_unique<ConstructorDeclaration>(std::move(parameters), std::move(body),
                                                    tokens[current_token]->get_span());
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
    const auto next_token = getNextToken();
    const auto span = tokens[current_token]->get_span();
    if (next_token->get_code() != IDENTIFIER)
    {
        throwError("identifier", getEnumName(tokens[current_token]->get_code()), span.get_line_num(),
                   span.get_pos_begin());
    }
    return std::make_unique<MethodName>(dynamic_cast<Identifier*>(next_token.get())->get_identifier(), span);
}

std::unique_ptr<Parameters> Parser::parseParameters()
{
    const auto next_token = peekNextToken();
    // No parameters
    if (next_token == RIGHT_PAREN)
    {
        return nullptr;
    }
    auto parameters = std::make_unique<Parameters>();
    parameters->parameters.push_back(parseParameter());
    while (peekNextToken() == COMMA)
    {
        consumeToken();
        parameters->parameters.push_back(parseParameter());
    }
    return parameters;
}

std::unique_ptr<Parameter> Parser::parseParameter()
{
    expect(IDENTIFIER);
    const auto identifier = getNextToken();
    expectAndConsume(COLON);
    auto className = parseClassName();
    return std::make_unique<Parameter>(identifier->to_string(), tokens[current_token]->get_span(),
                                       std::move(className));
}

std::unique_ptr<ReturnType> Parser::parseReturnType()
{
    // Implement the logic to parse the return type
    auto className = parseClassName();
    return std::make_unique<ReturnType>(std::move(className));
}


std::unique_ptr<VariableName> Parser::parseVariableName()
{
    const auto next_token = getNextToken();
    const auto span = tokens[current_token]->get_span();
    if (next_token->get_code() != IDENTIFIER)
    {
        throwError("identifier", getEnumName(tokens[current_token]->get_code()),
                   span.get_line_num(), span.get_pos_begin());
    }
    return std::make_unique<VariableName>(dynamic_cast<Identifier*>(next_token.get())->get_identifier(), span);
}


bool Parser::isPrimary(const TokenCode code)
{
    return code == INTEGER || code == REAL || code == BOOLEAN;
}
