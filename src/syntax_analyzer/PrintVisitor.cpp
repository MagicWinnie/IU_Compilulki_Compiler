//
// Created by nai1ka on 10/10/24.
//

#include <iostream>
#include "PrintVisitor.h"

void PrintVisitor::printIndentation() const
{
    for (int i = 0; i < indentationLevel; ++i)
    {
        out << "  "; // Two spaces for each indentation level
    }
}

PrintVisitor::PrintVisitor(std::ostream& out): out(out)
{
}


void PrintVisitor::visitLiterals(const Literals& node)
{
    printIndentation();
    out << "Literals" << std::endl;
    indentationLevel++;
    for (auto& literal : node.literals)
    {
        literal->accept(*this);
    }
    indentationLevel--;
}

void PrintVisitor::visitLiteral(const Literal& node)
{
    printIndentation();
    out << "Literal: " << node.value << std::endl;
}

void PrintVisitor::visitArguments(const Arguments& node)
{
    printIndentation();
    out << "Arguments" << std::endl;
    indentationLevel++;
    if (node.expressions) node.expressions->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitExpressions(const Expressions& node)
{
    printIndentation();
    out << "Expressions" << std::endl;
    indentationLevel++;
    for (auto& expression : node.expressions)
    {
        expression->accept(*this);
    }
    indentationLevel--;
}

void PrintVisitor::visitExpression(const Expression& node)
{
    printIndentation();
    out << "Expression" << std::endl;
    indentationLevel++;
    if (node.primary) node.primary->accept(*this);
    if (node.compoundExpression) node.compoundExpression->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitPrimary(const Primary& node)
{
    printIndentation();
    out << "Primary: " << node.value << std::endl;
    indentationLevel++;
    if (node.class_name) node.class_name->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitCompoundExpression(const CompoundExpression& node)
{
    printIndentation();
    out << "CompoundExpression: " << node.identifier << std::endl;
    indentationLevel++;
    if (node.arguments) node.arguments->accept(*this);
    for (auto& compoundExpression : node.compoundExpressions)
    {
        compoundExpression->accept(*this);
    }
    indentationLevel--;
}

void PrintVisitor::visitClassDeclaration(const ClassDeclaration& node)
{
    printIndentation();
    out << "ClassDeclaration" << std::endl;
    indentationLevel++;
    if (node.className) node.className->accept(*this);
    if (node.extension) node.extension->accept(*this);
    if (node.classBody) node.classBody->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitClassBody(const ClassBody& node)
{
    printIndentation();
    out << "ClassBody" << std::endl;
    indentationLevel++;
    if (node.memberDeclarations) node.memberDeclarations->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitExtension(const Extension& node)
{
    printIndentation();
    out << "Extension" << std::endl;
    indentationLevel++;
    if (node.className) node.className->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitBody(const Body& node)
{
    printIndentation();
    out << "Body" << std::endl;
    indentationLevel++;
    if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitBodyDeclarations(const BodyDeclarations& node)
{
    printIndentation();
    out << "BodyDeclarations" << std::endl;
    indentationLevel++;
    for (auto& bodyDeclaration : node.bodyDeclarations)
    {
        if (bodyDeclaration) bodyDeclaration->accept(*this);
    }
    indentationLevel--;
}

void PrintVisitor::visitBodyDeclaration(const BodyDeclaration& node)
{
    printIndentation();
    out << "BodyDeclaration" << std::endl;
    indentationLevel++;
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    if (node.statement) node.statement->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitStatement(const Statement& node)
{
    printIndentation();
    out << "Statement" << std::endl;
    indentationLevel++;
    if (node.assignment) node.assignment->accept(*this);
    if (node.expression) node.expression->accept(*this);
    if (node.ifStatement) node.ifStatement->accept(*this);
    if (node.whileLoop) node.whileLoop->accept(*this);
    if (node.returnStatement) node.returnStatement->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitIfStatement(const IfStatement& node)
{
    printIndentation();
    out << "IfStatement" << std::endl;
    indentationLevel++;
    if (node.expression) node.expression->accept(*this);
    if (node.ifBranch) node.ifBranch->accept(*this);
    if (node.elseBranch) node.elseBranch->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitIfBranch(const IfBranch& node)
{
    printIndentation();
    out << "IfBranch" << std::endl;
    indentationLevel++;
    if (node.body) node.body->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitElseBranch(const ElseBranch& node)
{
    printIndentation();
    out << "ElseBranch" << std::endl;
    indentationLevel++;
    if (node.body) node.body->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitWhileLoop(const WhileLoop& node)
{
    printIndentation();
    out << "WhileLoop" << std::endl;
    indentationLevel++;
    if (node.body) node.body->accept(*this);
    if (node.expression) node.expression->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitAssignment(const Assignment& node)
{
    printIndentation();
    out << "Assignment" << std::endl;
    indentationLevel++;
    if (node.expression) node.expression->accept(*this);
    if (node.variableName) node.variableName->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitMemberDeclarations(const MemberDeclarations& node)
{
    printIndentation();
    out << "MemberDeclarations" << std::endl;
    indentationLevel++;
    for (auto& memberDeclaration : node.member_declarations)
    {
        memberDeclaration->accept(*this);
    }
    indentationLevel--;
}

void PrintVisitor::visitMemberDeclaration(const MemberDeclaration& node)
{
    printIndentation();
    out << "MemberDeclaration" << std::endl;
    indentationLevel++;
    if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
    if (node.methodDeclaration) node.methodDeclaration->accept(*this);
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitConstructorDeclaration(const ConstructorDeclaration& node)
{
    printIndentation();
    out << "ConstructorDeclaration" << std::endl;
    indentationLevel++;
    if (node.body) node.body->accept(*this);
    if (node.parameters) node.parameters->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitReturnStatement(const ReturnStatement& node)
{
    printIndentation();
    out << "ReturnStatement" << std::endl;
    indentationLevel++;
    if (node.expression) node.expression->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitVariableDeclaration(const VariableDeclaration& node)
{
    printIndentation();
    out << "VariableDeclaration" << std::endl;
    indentationLevel++;
    if (node.expression) node.expression->accept(*this);
    if (node.variable) node.variable->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitMethodDeclaration(const MethodDeclaration& node)
{
    printIndentation();
    out << "MethodDeclaration" << std::endl;
    indentationLevel++;
    if (node.body) node.body->accept(*this);
    if (node.parameters) node.parameters->accept(*this);
    if (node.returnType) node.returnType->accept(*this);
    if (node.methodName) node.methodName->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitMethodName(const MethodName& node)
{
    printIndentation();
    out << "MethodName: " << node.name << std::endl;
}

void PrintVisitor::visitParameters(const Parameters& node)
{
    printIndentation();
    out << "Parameters" << std::endl;
    indentationLevel++;
    for (auto& parameter : node.parameters)
    {
        parameter->accept(*this);
    }
    indentationLevel--;
}

void PrintVisitor::visitParameter(const Parameter& node)
{
    printIndentation();
    out << "Parameter: " << node.name << std::endl;
    indentationLevel++;
    if (node.className) node.className->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitReturnType(const ReturnType& node)
{
    printIndentation();
    out << "ReturnType" << std::endl;
    indentationLevel++;
    if (node.className) node.className->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitVariableName(const VariableName& node)
{
    printIndentation();
    out << "VariableName: " << node.name << std::endl;
}

void PrintVisitor::visitProgram(const Program& node)
{
    printIndentation();
    out << "Program" << std::endl;
    indentationLevel++;
    if (node.programDeclaration) node.programDeclaration->accept(*this);
    if (node.classDeclarations) node.classDeclarations->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitProgramDeclaration(const ProgramDeclaration& node)
{
    printIndentation();
    out << "ProgramDeclaration" << std::endl;
    indentationLevel++;
    if (node.className) node.className->accept(*this);
    if (node.arguments) node.arguments->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitClassName(const ClassName& node)
{
    printIndentation();
    out << "ClassName: " << node.name << std::endl;
}

void PrintVisitor::visitProgramArguments(const ProgramArguments& node)
{
    printIndentation();
    out << "ProgramArguments" << std::endl;
    indentationLevel++;
    if (node.literals) node.literals->accept(*this);
    indentationLevel--;
}

void PrintVisitor::visitClassDeclarations(const ClassDeclarations& node)
{
    printIndentation();
    out << "ClassDeclarations" << std::endl;
    indentationLevel++;
    for (auto& classDeclaration : node.classDeclarations)
    {
        classDeclaration->accept(*this);
    }
    indentationLevel--;
}
