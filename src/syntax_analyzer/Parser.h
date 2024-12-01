//
// Created by Nail Minnemullin on 08.10.2024.
//
#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include "../lexical_analyzer/token.h"
#include "AST.h"

class Parser
{
    std::string infile_path;
    bool debug;
    std::vector<std::unique_ptr<Token>> tokens;
    int current_token = 0;

public:
    explicit Parser(std::vector<std::unique_ptr<Token>>, const std::string&, const bool&);

    std::unique_ptr<Program> parse();

    void expectAndConsume(TokenCode);
    void expect(TokenCode) const;

    [[nodiscard]] TokenCode peekNextToken(int offset = 0) const;

    std::unique_ptr<Token> getNextToken();

    void consumeToken();

    void toFile(const std::string&, const std::unique_ptr<Program>&) const;

    std::unique_ptr<Program> parseProgram();

    std::unique_ptr<ProgramDeclaration> parseProgramDeclaration();
    std::unique_ptr<ProgramArguments> parseProgramArguments();
    std::unique_ptr<Literals> parseLiterals();
    std::unique_ptr<Literal> parseLiteral();
    std::unique_ptr<Arguments> parseArguments();
    std::unique_ptr<Expressions> parseExpressions();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<ReturnStatement> parseReturnStatement();
    std::unique_ptr<Primary> parsePrimary();
    std::unique_ptr<CompoundExpression> parseCompoundExpression();
    std::unique_ptr<ClassDeclarations> parseClassDeclarations();
    std::unique_ptr<ClassDeclaration> parseClassDeclaration();
    std::unique_ptr<ClassBody> parseClassBody(const std::string&);
    std::unique_ptr<ClassName> parseClassName();
    std::unique_ptr<Extension> parseExtension();
    std::unique_ptr<Body> parseBody();
    std::unique_ptr<BodyDeclarations> parseBodyDeclarations();
    std::unique_ptr<BodyDeclaration> parseBodyDeclaration();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<IfStatement> parseIfStatement();
    std::unique_ptr<IfBranch> parseIfBranch();
    std::unique_ptr<ElseBranch> parseElseBranch();
    std::unique_ptr<WhileLoop> parseWhileLoop();
    std::unique_ptr<Assignment> parseAssignment();
    std::unique_ptr<MemberDeclarations> parseMemberDeclarations(const std::string&);
    std::unique_ptr<MemberDeclaration> parseMemberDeclaration(const std::string&);
    std::unique_ptr<ConstructorDeclaration> parseConstructorDeclaration(const std::string&);
    std::unique_ptr<VariableDeclaration> parseVariableDeclaration();
    std::unique_ptr<MethodDeclaration> parseMethodDeclaration();
    std::unique_ptr<MethodName> parseMethodName();
    std::unique_ptr<Parameters> parseParameters();
    std::unique_ptr<Parameter> parseParameter();
    std::unique_ptr<ReturnType> parseReturnType();
    std::unique_ptr<VariableName> parseVariableName();
    static bool isPrimary(TokenCode);
};
