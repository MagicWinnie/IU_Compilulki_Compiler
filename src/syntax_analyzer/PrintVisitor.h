//
// Created by nai1ka on 10/10/24.
//
#pragma once

#include "AST.h"

class PrintVisitor final : public Visitor
{
    std::ostream& out;
    int indentationLevel = 0;

    // Helper function to print indentation
    void printIndentation() const;

public:
    explicit PrintVisitor(std::ostream& out);
    void visitProgram(Program&) override;
    void visitProgramDeclaration(ProgramDeclaration&) override;
    void visitClassName(ClassName&) override;
    void visitProgramArguments(ProgramArguments&) override;
    void visitLiterals(Literals&) override;
    void visitLiteral(Literal&) override;
    void visitArguments(Arguments&) override;
    void visitExpressions(Expressions&) override;
    void visitExpression(Expression&) override;
    void visitPrimary(Primary&) override;
    void visitCompoundExpression(CompoundExpression&) override;
    void visitClassDeclarations(ClassDeclarations&) override;
    void visitClassDeclaration(ClassDeclaration&) override;
    void visitClassBody(ClassBody&) override;
    void visitExtension(Extension&) override;
    void visitBody(Body&) override;
    void visitBodyDeclarations(BodyDeclarations&) override;
    void visitBodyDeclaration(BodyDeclaration&) override;
    void visitStatement(Statement&) override;
    void visitIfStatement(IfStatement&) override;
    void visitIfBranch(IfBranch&) override;
    void visitElseBranch(ElseBranch&) override;
    void visitWhileLoop(WhileLoop&) override;
    void visitAssignment(Assignment&) override;
    void visitMemberDeclarations(MemberDeclarations&) override;
    void visitMemberDeclaration(MemberDeclaration&) override;
    void visitConstructorDeclaration(ConstructorDeclaration&) override;
    void visitReturnStatement(ReturnStatement&) override;
    void visitVariableDeclaration(VariableDeclaration&) override;
    void visitMethodDeclaration(MethodDeclaration&) override;
    void visitMethodName(MethodName&) override;
    void visitParameters(Parameters&) override;
    void visitParameter(Parameter&) override;
    void visitReturnType(ReturnType&) override;
    void visitVariableName(VariableName&) override;

    // Implement other visit methods for different types
};
