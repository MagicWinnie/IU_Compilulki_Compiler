//
// Created by nai1ka on 10/10/24.
//

#ifndef PRINTVISITOR_H
#define PRINTVISITOR_H
#include "AST.h"

class PrintVisitor final : public Visitor
{
    std::ostream& out;
    int indentationLevel = 0;

    // Helper function to print indentation
    void printIndentation() const;

public:
    explicit PrintVisitor(std::ostream& out);
    void visitProgram(const Program&) override;
    void visitProgramDeclaration(const ProgramDeclaration&) override;
    void visitClassName(const ClassName&) override;
    void visitProgramArguments(const ProgramArguments&) override;
    void visitLiterals(const Literals&) override;
    void visitLiteral(const Literal&) override;
    void visitArguments(const Arguments&) override;
    void visitExpressions(const Expressions&) override;
    void visitExpression(const Expression&) override;
    void visitPrimary(const Primary&) override;
    void visitCompoundExpression(const CompoundExpression&) override;
    void visitClassDeclarations(const ClassDeclarations&) override;
    void visitClassDeclaration(const ClassDeclaration&) override;
    void visitClassBody(const ClassBody&) override;
    void visitExtension(const Extension&) override;
    void visitBody(const Body&) override;
    void visitBodyDeclarations(const BodyDeclarations&) override;
    void visitBodyDeclaration(const BodyDeclaration&) override;
    void visitStatement(const Statement&) override;
    void visitIfStatement(const IfStatement&) override;
    void visitIfBranch(const IfBranch&) override;
    void visitElseBranch(const ElseBranch&) override;
    void visitWhileLoop(const WhileLoop&) override;
    void visitAssignment(const Assignment&) override;
    void visitMemberDeclarations(const MemberDeclarations&) override;
    void visitMemberDeclaration(const MemberDeclaration&) override;
    void visitConstructorDeclaration(const ConstructorDeclaration&) override;
    void visitReturnStatement(const ReturnStatement&) override;
    void visitVariableDeclaration(const VariableDeclaration&) override;
    void visitMethodDeclaration(const MethodDeclaration&) override;
    void visitMethodName(const MethodName&) override;
    void visitParameters(const Parameters&) override;
    void visitParameter(const Parameter&) override;
    void visitReturnType(const ReturnType&) override;
    void visitVariableName(const VariableName&) override;

    // Implement other visit methods for different types
};

#endif //PRINTVISITOR_H
