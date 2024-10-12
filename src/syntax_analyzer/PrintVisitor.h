//
// Created by nai1ka on 10/10/24.
//

#ifndef PRINTVISITOR_H
#define PRINTVISITOR_H
#include "AST.h"

class PrintVisitor : public Visitor {
private:
    int indentationLevel = 0;

    // Helper function to print indentation
    void printIndentation() const {
        for (int i = 0; i < indentationLevel; ++i) {
            std::cout << "  ";  // Two spaces for each indentation level
        }
    }

public:
    void visit(const Literals& node) override {
        printIndentation();
        std::cout << "Literals" << std::endl;
        indentationLevel++;
        for (auto& literal : node.literals) {
            literal->accept(*this);
        }
        indentationLevel--;
    }

    void visit(const Literal& node) override {
        printIndentation();
        std::cout << "Literal: " << node.value << std::endl;
    }

    void visit(const Arguments& node) override {
        printIndentation();
        std::cout << "Arguments" << std::endl;
        indentationLevel++;
        if (node.expressions) node.expressions->accept(*this);
        indentationLevel--;
    }

    void visit(const Expressions& node) override {
        printIndentation();
        std::cout << "Expressions" << std::endl;
        indentationLevel++;
        for (auto& expression : node.expressions) {
            expression->accept(*this);
        }
        indentationLevel--;
    }

    void visit(const Expression& node) override {
        printIndentation();
        std::cout << "Expression" << std::endl;
        indentationLevel++;
        if (node.primary) node.primary->accept(*this);
        if (node.compoundExpression) node.compoundExpression->accept(*this);
        indentationLevel--;
    }

    void visit(const Primary& node) override {
        printIndentation();
        std::cout << "Primary: " << node.value << std::endl;
        indentationLevel++;
        if (node.class_name) node.class_name->accept(*this);
        indentationLevel--;
    }

    void visit(const CompoundExpression& node) override {
        printIndentation();
        std::cout << "CompoundExpression: " << node.identifier << std::endl;
        indentationLevel++;
        if (node.arguments) node.arguments->accept(*this);
        for(auto& compoundExpression: node.compoundExpressions)
        {
            compoundExpression->accept(*this);
        }
        indentationLevel--;
    }

    void visit(const ClassDeclaration& node) override {
        printIndentation();
        std::cout << "ClassDeclaration" << std::endl;
        indentationLevel++;
        if (node.className) node.className->accept(*this);
        if (node.extension) node.extension->accept(*this);
        if (node.classBody) node.classBody->accept(*this);
        indentationLevel--;
    }

    void visit(const ClassBody& node) override {
        printIndentation();
        std::cout << "ClassBody" << std::endl;
        indentationLevel++;
        if (node.memberDeclarations) node.memberDeclarations->accept(*this);
        indentationLevel--;
    }

    void visit(const Extension& node) override {
        printIndentation();
        std::cout << "Extension" << std::endl;
        indentationLevel++;
        if (node.className) node.className->accept(*this);
        indentationLevel--;
    }

    void visit(const Body& node) override {
        printIndentation();
        std::cout << "Body" << std::endl;
        indentationLevel++;
        if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
        indentationLevel--;
    }

    void visit(const BodyDeclarations& node) override {
        printIndentation();
        std::cout << "BodyDeclarations" << std::endl;
        indentationLevel++;
        for (auto& bodyDeclaration : node.bodyDeclarations) {
            bodyDeclaration->accept(*this);
        }
        indentationLevel--;
    }

    void visit(const BodyDeclaration& node) override {
        printIndentation();
        std::cout << "BodyDeclaration" << std::endl;
        indentationLevel++;
        if (node.variableDeclaration) node.variableDeclaration->accept(*this);
        if (node.statement) node.statement->accept(*this);
        indentationLevel--;
    }

    void visit(const Statement& node) override {
        printIndentation();
        std::cout << "Statement" << std::endl;
        indentationLevel++;
        if (node.assignment) node.assignment->accept(*this);
        if (node.expression) node.expression->accept(*this);
        if (node.ifStatement) node.ifStatement->accept(*this);
        if (node.whileLoop) node.whileLoop->accept(*this);
        if (node.returnStatement) node.returnStatement->accept(*this);
        indentationLevel--;
    }

    void visit(const IfStatement& node) override {
        printIndentation();
        std::cout << "IfStatement" << std::endl;
        indentationLevel++;
        if (node.expression) node.expression->accept(*this);
        if (node.ifBranch) node.ifBranch->accept(*this);
        if (node.elseBranch) node.elseBranch->accept(*this);
        indentationLevel--;
    }

    void visit(const IfBranch& node) override {
        printIndentation();
        std::cout << "IfBranch" << std::endl;
        indentationLevel++;
        if (node.body) node.body->accept(*this);
        indentationLevel--;
    }

    void visit(const ElseBranch& node) override {
        printIndentation();
        std::cout << "ElseBranch" << std::endl;
        indentationLevel++;
        if (node.body) node.body->accept(*this);
        indentationLevel--;
    }

    void visit(const WhileLoop& node) override {
        printIndentation();
        std::cout << "WhileLoop" << std::endl;
        indentationLevel++;
        if (node.body) node.body->accept(*this);
        if (node.expression) node.expression->accept(*this);
        indentationLevel--;
    }

    void visit(const Assignment& node) override {
        printIndentation();
        std::cout << "Assignment" << std::endl;
        indentationLevel++;
        if (node.expression) node.expression->accept(*this);
        if (node.variableName) node.variableName->accept(*this);
        indentationLevel--;
    }

    void visit(const MemberDeclarations& node) override {
        printIndentation();
        std::cout << "MemberDeclarations" << std::endl;
        indentationLevel++;
        for (auto& memberDeclaration : node.member_declarations) {
            memberDeclaration->accept(*this);
        }
        indentationLevel--;
    }

    void visit(const MemberDeclaration& node) override {
        printIndentation();
        std::cout << "MemberDeclaration" << std::endl;
        indentationLevel++;
        if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
        indentationLevel--;
    }

    void visit(const ConstructorDeclaration& node) override {
        printIndentation();
        std::cout << "ConstructorDeclaration" << std::endl;
        indentationLevel++;
        if (node.body) node.body->accept(*this);
        if (node.parameters) node.parameters->accept(*this);
        indentationLevel--;
    }

    void visit(const ReturnStatement& node) override {
        printIndentation();
        std::cout << "ReturnStatement" << std::endl;
        indentationLevel++;
        if (node.expression) node.expression->accept(*this);
        indentationLevel--;
    }

    void visit(const VariableDeclaration& node) override {
        printIndentation();
        std::cout << "VariableDeclaration" << std::endl;
        indentationLevel++;
        if (node.expression) node.expression->accept(*this);
        if (node.variable) node.variable->accept(*this);
        indentationLevel--;
    }

    void visit(const MethodDeclaration& node) override {
        printIndentation();
        std::cout << "MethodDeclaration" << std::endl;
        indentationLevel++;
        if (node.body) node.body->accept(*this);
        if (node.parameters) node.parameters->accept(*this);
        if (node.returnType) node.returnType->accept(*this);
        if (node.methodName) node.methodName->accept(*this);
        indentationLevel--;
    }

    void visit(const MethodName& node) override {
        printIndentation();
        std::cout << "MethodName: " << node.name << std::endl;
    }

    void visit(const Parameters& node) override {
        printIndentation();
        std::cout << "Parameters" << std::endl;
        indentationLevel++;
        for (auto& parameter : node.parameters) {
            parameter->accept(*this);
        }
        indentationLevel--;
    }

    void visit(const Parameter& node) override {
        printIndentation();
        std::cout << "Parameter: " << node.name << std::endl;
        indentationLevel++;
        if (node.className) node.className->accept(*this);
        indentationLevel--;
    }

    void visit(const ReturnType& node) override {
        printIndentation();
        std::cout << "ReturnType" << std::endl;
        indentationLevel++;
        if (node.className) node.className->accept(*this);
        indentationLevel--;
    }

    void visit(const VariableName& node) override {
        printIndentation();
        std::cout << "VariableName: " << node.name << std::endl;
    }

    void visit(const Program& node) override {
        printIndentation();
        std::cout << "Program" << std::endl;
        indentationLevel++;
        if (node.programDeclaration) node.programDeclaration->accept(*this);
        if (node.classDeclarations) node.classDeclarations->accept(*this);
        indentationLevel--;
    }

    void visit(const ProgramDeclaration& node) override {
        printIndentation();
        std::cout << "ProgramDeclaration" << std::endl;
        indentationLevel++;
        if (node.className) node.className->accept(*this);
        if (node.arguments) node.arguments->accept(*this);
        indentationLevel--;
    }

    void visit(const ClassName& node) override {
        printIndentation();
        std::cout << "ClassName: " << node.name << std::endl;

    }

    void visit(const ProgramArguments& node) override {
        printIndentation();
        std::cout << "ProgramArguments" << std::endl;
        indentationLevel++;
        if(node.literals) node.literals->accept(*this);
        indentationLevel--;
    }

    void visit(const ClassDeclarations& node) override {
        printIndentation();
        std::cout << "ClassDeclarations" << std::endl;
        indentationLevel++;
        for (auto& classDeclaration : node.classDeclarations) {
            classDeclaration->accept(*this);
        }
        indentationLevel--;
    }

    // Implement other visit methods for different node types
};

#endif //PRINTVISITOR_H
