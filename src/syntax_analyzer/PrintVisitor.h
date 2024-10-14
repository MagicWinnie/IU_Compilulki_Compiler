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

    void visit(const Literals& node) override;

    void visit(const Literal& node) override;

    void visit(const Arguments& node) override;

    void visit(const Expressions& node) override;

    void visit(const Expression& node) override;

    void visit(const Primary& node) override;

    void visit(const CompoundExpression& node) override;

    void visit(const ClassDeclaration& node) override;

    void visit(const ClassBody& node) override;

    void visit(const Extension& node) override;

    void visit(const Body& node) override;

    void visit(const BodyDeclarations& node) override;

    void visit(const BodyDeclaration& node) override;

    void visit(const Statement& node) override;

    void visit(const IfStatement& node) override;

    void visit(const IfBranch& node) override;

    void visit(const ElseBranch& node) override;

    void visit(const WhileLoop& node) override;

    void visit(const Assignment& node) override;

    void visit(const MemberDeclarations& node) override;

    void visit(const MemberDeclaration& node) override;

    void visit(const ConstructorDeclaration& node) override;

    void visit(const ReturnStatement& node) override;

    void visit(const VariableDeclaration& node) override;

    void visit(const MethodDeclaration& node) override;

    void visit(const MethodName& node) override;

    void visit(const Parameters& node) override;

    void visit(const Parameter& node) override;

    void visit(const ReturnType& node) override;

    void visit(const VariableName& node) override;

    void visit(const Program& node) override;

    void visit(const ProgramDeclaration& node) override;

    void visit(const ClassName& node) override;

    void visit(const ProgramArguments& node) override;

    void visit(const ClassDeclarations& node) override;

    // Implement other visit methods for different node types
};

#endif //PRINTVISITOR_H
