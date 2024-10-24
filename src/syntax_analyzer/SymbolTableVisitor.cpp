//
// Created by nai1ka on 10/23/24.
//

#include "AST.h"
#include "SymbolTable.cpp"

class SymbolTableVisitor : public Visitor
{
private:
    ScopedSymbolTable symbolTable;

public:
    SymbolTableVisitor()
    {

    }

    ~SymbolTableVisitor()
    {
        symbolTable.leaveScope();
    }

    void visitProgram(const Program& node) override
    {
        if (node.programDeclaration) node.programDeclaration->accept(*this);
        if (node.classDeclarations) node.classDeclarations->accept(*this);
    }

    void visitProgramDeclaration(const ProgramDeclaration& node) override
    {
        symbolTable.addEntry(node.className->name, "ProgramType");
        if (node.className) node.className->accept(*this);
        if (node.arguments) node.arguments->accept(*this);
    }

    void visitClassName(const ClassName& node) override
    {

    }

    void visitProgramArguments(const ProgramArguments& node) override
    {
        if (node.literals) node.literals->accept(*this);
    }

    void visitLiterals(const Literals& node) override
    {
        for (auto& literal : node.literals)
        {
            literal->accept(*this);
        }
    }

    void visitLiteral(const Literal& node) override
    {

    }

    void visitArguments(const Arguments& node) override
    {
        if (node.expressions) node.expressions->accept(*this);
    }

    void visitExpressions(const Expressions& node) override
    {
        for (auto& expression : node.expressions)
        {
            expression->accept(*this);
        }
    }

    void visitExpression(const Expression& node) override
    {
        if (node.primary) node.primary->accept(*this);
        if (node.compoundExpression) node.compoundExpression->accept(*this);
    }

    void visitPrimary(const Primary& node) override
    {
        if (node.class_name) node.class_name->accept(*this);
    }

    void visitCompoundExpression(const CompoundExpression& node) override
    {
        if (node.arguments) node.arguments->accept(*this);
        for (auto& compoundExpression : node.compoundExpressions)
        {
            compoundExpression->accept(*this);
        }
    }

    void visitClassDeclarations(const ClassDeclarations& node) override
    {

        for (auto& classDeclaration : node.classDeclarations)
        {
            classDeclaration->accept(*this);
        }
    }

    void visitClassDeclaration(const ClassDeclaration& node) override
    {
        symbolTable.enterScope();
        if (node.className) node.className->accept(*this);
        if (node.extension) node.extension->accept(*this);
        if (node.classBody) node.classBody->accept(*this);
        symbolTable.leaveScope();
        // TODO add leaveScope in other places
    }

    void visitClassBody(const ClassBody& node) override
    {
        if (node.memberDeclarations) node.memberDeclarations->accept(*this);
    }

    void visitExtension(const Extension& node) override
    {
        if (node.className) node.className->accept(*this);
    }

    void visitBody(const Body& node) override
    {
        if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
    }

    void visitBodyDeclarations(const BodyDeclarations& node) override
    {
        for (auto& bodyDeclaration : node.bodyDeclarations)
        {
            if (bodyDeclaration) bodyDeclaration->accept(*this);
        }
    }

    void visitBodyDeclaration(const BodyDeclaration& node) override
    {
        if (node.variableDeclaration) node.variableDeclaration->accept(*this);
        if (node.statement) node.statement->accept(*this);
    }

    void visitStatement(const Statement& node) override
    {
        if (node.assignment) node.assignment->accept(*this);
        if (node.expression) node.expression->accept(*this);
        if (node.ifStatement) node.ifStatement->accept(*this);
        if (node.whileLoop) node.whileLoop->accept(*this);
        if (node.returnStatement) node.returnStatement->accept(*this);
    }

    void visitIfStatement(const IfStatement& node) override
    {
        if (node.expression) node.expression->accept(*this);
        if (node.ifBranch) node.ifBranch->accept(*this);
        if (node.elseBranch) node.elseBranch->accept(*this);
    }

    void visitIfBranch(const IfBranch& node) override
    {
        if (node.body) node.body->accept(*this);
    }

    void visitElseBranch(const ElseBranch& node) override
    {
        if (node.body) node.body->accept(*this);
    }

    void visitWhileLoop(const WhileLoop& node) override
    {
        if (node.body) node.body->accept(*this);
        if (node.expression) node.expression->accept(*this);
    }

    void visitAssignment(const Assignment& node) override
    {
        if (node.expression) node.expression->accept(*this);
        if (node.variableName) node.variableName->accept(*this);
    }

    void visitMemberDeclarations(const MemberDeclarations& node) override
    {
        for (auto& memberDeclaration : node.member_declarations)
        {
            memberDeclaration->accept(*this);
        }
    }

    void visitMemberDeclaration(const MemberDeclaration& node) override
    {
        if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
        if (node.methodDeclaration) node.methodDeclaration->accept(*this);
        if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    }

    void visitConstructorDeclaration(const ConstructorDeclaration& node) override
    {
        if (node.body) node.body->accept(*this);
        if (node.parameters) node.parameters->accept(*this);
    }

    void visitReturnStatement(const ReturnStatement& node) override
    {
        if (node.expression) node.expression->accept(*this);
    }

    void visitVariableDeclaration(const VariableDeclaration& node) override
    {
        symbolTable.addEntry(node.variable->name, "VariableType");
        if (node.expression) node.expression->accept(*this);
        if (node.variable) node.variable->accept(*this);
    }

    void visitMethodDeclaration(const MethodDeclaration& node) override
    {
        if (node.body) node.body->accept(*this);
        if (node.parameters) node.parameters->accept(*this);
        if (node.returnType) node.returnType->accept(*this);
        if (node.methodName) node.methodName->accept(*this);
    }

    void visitMethodName(const MethodName& node) override
    {

    }

    void visitParameters(const Parameters& node) override
    {
        for (auto& parameter : node.parameters)
        {
            parameter->accept(*this);
        }
    }

    void visitParameter(const Parameter& node) override
    {
        if (node.className) node.className->accept(*this);
    }

    void visitReturnType(const ReturnType& node) override
    {

    }
    void visitVariableName(const VariableName& node) override
    {
        try {
            symbolTable.lookup(node.name);
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    // Implement other visit methods as needed
};
