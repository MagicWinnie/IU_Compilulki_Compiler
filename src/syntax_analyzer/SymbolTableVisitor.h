//
// Created by nai1ka on 10/23/24.
//
#pragma once

#include <iostream>
#include "AST.h"
#include "SymbolTable.h"

class SymbolTableVisitor final : public Visitor
{
    ScopedSymbolTable symbolTable;

public:
    SymbolTableVisitor();

    ~SymbolTableVisitor() override;

    void visitProgram(const Program& node) override;

    void visitProgramDeclaration(const ProgramDeclaration& node) override;

    void visitClassName(const ClassName& node) override;

    void visitProgramArguments(const ProgramArguments& node) override;

    void visitLiterals(const Literals& node) override;

    void visitLiteral(const Literal& node) override;

    void visitArguments(const Arguments& node) override;

    void visitExpressions(const Expressions& node) override;

    void visitExpression(const Expression& node) override;

    void visitPrimary(const Primary& node) override;

    void visitCompoundExpression(const CompoundExpression& node) override;

    void visitClassDeclarations(const ClassDeclarations& node) override;

    void visitClassDeclaration(const ClassDeclaration& node) override;

    void visitClassBody(const ClassBody& node) override;

    void visitExtension(const Extension& node) override;

    void visitBody(const Body& node) override;

    void visitBodyDeclarations(const BodyDeclarations& node) override;

    void visitBodyDeclaration(const BodyDeclaration& node) override;

    void visitStatement(const Statement& node) override;

    void visitIfStatement(const IfStatement& node) override;

    void visitIfBranch(const IfBranch& node) override;

    void visitElseBranch(const ElseBranch& node) override;

    void visitWhileLoop(const WhileLoop& node) override;

    void visitAssignment(const Assignment& node) override;

    void visitMemberDeclarations(const MemberDeclarations& node) override;

    void visitMemberDeclaration(const MemberDeclaration& node) override;

    void visitConstructorDeclaration(const ConstructorDeclaration& node) override;

    void visitReturnStatement(const ReturnStatement& node) override;

    void visitVariableDeclaration(const VariableDeclaration& node) override;

    void visitMethodDeclaration(const MethodDeclaration& node) override;

    void visitMethodName(const MethodName& node) override;

    void visitParameters(const Parameters& node) override;

    void visitParameter(const Parameter& node) override;

    void visitReturnType(const ReturnType& node) override;

    void visitVariableName(const VariableName& node) override;

    // Implement other visit methods as needed
};
