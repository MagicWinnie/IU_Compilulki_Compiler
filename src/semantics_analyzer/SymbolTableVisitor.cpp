#include <iostream>
#include <stdexcept>
#include "SymbolTableVisitor.h"

SymbolTableVisitor::SymbolTableVisitor() = default;

SymbolTableVisitor::~SymbolTableVisitor()
{
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitProgram(const Program& node)
{
    symbolTable.enterScope();
    if (node.programDeclaration) node.programDeclaration->accept(*this);
    if (node.classDeclarations) node.classDeclarations->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitProgramDeclaration(const ProgramDeclaration& node)
{
    // TODO handle classes
    // symbolTable.addClassEntry(node.className->name, "ProgramType");
    if (node.className) node.className->accept(*this);
    if (node.arguments) node.arguments->accept(*this);
}

void SymbolTableVisitor::visitClassName(const ClassName& node)
{
}

void SymbolTableVisitor::visitProgramArguments(const ProgramArguments& node)
{
    if (node.literals) node.literals->accept(*this);
}

void SymbolTableVisitor::visitLiterals(const Literals& node)
{
    for (auto& literal : node.literals)
    {
        literal->accept(*this);
    }
}

void SymbolTableVisitor::visitLiteral(const Literal& node)
{
}

void SymbolTableVisitor::visitArguments(const Arguments& node)
{
    if (node.expressions) node.expressions->accept(*this);
}

void SymbolTableVisitor::visitExpressions(const Expressions& node)
{
    for (auto& expression : node.expressions)
    {
        expression->accept(*this);
    }
}

void SymbolTableVisitor::visitExpression(const Expression& node)
{
    if (node.primary) node.primary->accept(*this);
    if (node.compoundExpression) node.compoundExpression->accept(*this);
}

void SymbolTableVisitor::visitPrimary(const Primary& node)
{
    if (node.class_name) node.class_name->accept(*this);
}

void SymbolTableVisitor::visitCompoundExpression(const CompoundExpression& node)
{
    if (node.arguments) node.arguments->accept(*this);
    for (auto& compoundExpression : node.compoundExpressions)
    {
        compoundExpression->accept(*this);
    }
}

void SymbolTableVisitor::visitClassDeclarations(const ClassDeclarations& node)
{
    for (auto& classDeclaration : node.classDeclarations)
    {
        classDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitClassDeclaration(const ClassDeclaration& node)
{
    symbolTable.enterScope();
    symbolTable.addClassEntry(node.className->name, node.className->span);
    if (node.className) node.className->accept(*this);
    if (node.extension) node.extension->accept(*this);
    if (node.classBody) node.classBody->accept(*this);
    symbolTable.leaveScope();
    // TODO add leaveScope in other places
}

void SymbolTableVisitor::visitClassBody(const ClassBody& node)
{
    if (node.memberDeclarations) node.memberDeclarations->accept(*this);
}

void SymbolTableVisitor::visitExtension(const Extension& node)
{
    if (node.className) node.className->accept(*this);
}

void SymbolTableVisitor::visitBody(const Body& node)
{
    if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
}

void SymbolTableVisitor::visitBodyDeclarations(const BodyDeclarations& node)
{
    for (auto& bodyDeclaration : node.bodyDeclarations)
    {
        if (bodyDeclaration) bodyDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitBodyDeclaration(const BodyDeclaration& node)
{
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    if (node.statement) node.statement->accept(*this);
}

void SymbolTableVisitor::visitStatement(const Statement& node)
{
    if (node.assignment) node.assignment->accept(*this);
    if (node.expression) node.expression->accept(*this);
    if (node.ifStatement) node.ifStatement->accept(*this);
    if (node.whileLoop) node.whileLoop->accept(*this);
    if (node.returnStatement) node.returnStatement->accept(*this);
}

void SymbolTableVisitor::visitIfStatement(const IfStatement& node)
{
    if (node.expression) node.expression->accept(*this);
    if (node.ifBranch) node.ifBranch->accept(*this);
    if (node.elseBranch) node.elseBranch->accept(*this);
}

void SymbolTableVisitor::visitIfBranch(const IfBranch& node)
{
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitElseBranch(const ElseBranch& node)
{
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitWhileLoop(const WhileLoop& node)
{
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    if (node.expression) node.expression->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitAssignment(const Assignment& node)
{
    // [CHECK] if variable is declared
    try
    {
        symbolTable.lookupVariable(node.variableName->name, node.variableName->span);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    // [CHECK] if variable is assigned to the correct type
    auto variableType = symbolTable.lookupVariable(node.variableName->name, node.variableName->span);
    // TODO firstly check if we assign a variable or a class or a method
    // auto expressionType = node.expression->compoundExpression->identifier;
    // if (variableType != expressionType)
    // {
    //     throw std::runtime_error(
    //         "Variable " + node.variableName->name + " is of type " + variableType +
    //         " but is being assigned to type " + expressionType);
    // }

    if (node.expression) node.expression->accept(*this);
    if (node.variableName) node.variableName->accept(*this);
}

void SymbolTableVisitor::visitMemberDeclarations(const MemberDeclarations& node)
{
    for (auto& memberDeclaration : node.member_declarations)
    {
        memberDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitMemberDeclaration(const MemberDeclaration& node)
{
    if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
    if (node.methodDeclaration) node.methodDeclaration->accept(*this);
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
}

void SymbolTableVisitor::visitConstructorDeclaration(const ConstructorDeclaration& node)
{
    symbolTable.enterScope();
    if (node.parameters) node.parameters->accept(*this);
    auto paramNames = std::vector<std::string>();

    // Add parameters to the symbol table
    if (node.parameters)
    {
        for (const auto& parameter : node.parameters->parameters)
        {
            paramNames.push_back(parameter->className->name);
            symbolTable.addVariableEntry(parameter->name, parameter->className->name, parameter->span);
        }
    }

    symbolTable.addFunctionEntry("this", "void", node.span, paramNames);
    if (node.body) node.body->accept(*this);

    std::unique_ptr<ReturnStatement> returnStatement = nullptr;
    for (const auto& bodyDeclaration : node.body->bodyDeclarations->bodyDeclarations)
    {
        if (bodyDeclaration && bodyDeclaration->statement && bodyDeclaration->statement->returnStatement)
        {
            returnStatement = std::move(bodyDeclaration->statement->returnStatement);
        }
    }


    // [CHECK] if constructor has return statement
    if (returnStatement != nullptr)
    {
        throw std::runtime_error("Constructor cannot have return statement");
    }

    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitReturnStatement(const ReturnStatement& node)
{
    if (node.expression) node.expression->accept(*this);
}

void SymbolTableVisitor::visitVariableDeclaration(const VariableDeclaration& node)
{
    if (node.expression->primary)
    {
        //symbolTable.addVariableEntry(node.variable->name, node.expression->);
    }
    else if (node.expression->compoundExpression)
    {
        symbolTable.addVariableEntry(node.variable->name, node.expression->compoundExpression->identifier,
                                     node.variable->span);
    }

    if (node.expression) node.expression->accept(*this);
    if (node.variable) node.variable->accept(*this);
}

void SymbolTableVisitor::visitMethodDeclaration(const MethodDeclaration& node)
{
    symbolTable.enterScope();
    auto paramNames = std::vector<std::string>();

    if (node.parameters)
    {
        for (const auto& parameter : node.parameters->parameters)
        {
            paramNames.push_back(parameter->className->name);
            symbolTable.addVariableEntry(parameter->name, parameter->className->name, parameter->span);
        }
    }

    symbolTable.addFunctionEntry(
        node.methodName->name,
        node.returnType ? node.returnType->className->name : "void",
        node.methodName->span,
        paramNames
    );

    if (node.body) node.body->accept(*this);
    if (node.parameters) node.parameters->accept(*this);

    // [CHECK] if return type is correct
    const auto expectedReturnType = symbolTable.lookupFunction(node.methodName->name, node.methodName->span).returnType;
    std::unique_ptr<ReturnStatement> returnStatement = nullptr;
    for (const auto& bodyDeclaration : node.body->bodyDeclarations->bodyDeclarations)
    {
        if (bodyDeclaration && bodyDeclaration->statement && bodyDeclaration->statement->returnStatement)
        {
            returnStatement = std::move(bodyDeclaration->statement->returnStatement);
        }
    }


    if (returnStatement == nullptr)
    {
        if (expectedReturnType != "void")
        {
            throw std::runtime_error(
                "Method " + node.methodName->name + " is of type " + expectedReturnType +
                " but is being assigned to type void");
        }
    }
    else
    {
        // TODO check type of the last element in the compound expression
        const auto span = returnStatement->expression->compoundExpression->span;
        const auto returnVariableType = symbolTable.lookupVariable(
            returnStatement->expression->compoundExpression->identifier,
            span
        );
        if (expectedReturnType != returnVariableType)
        {
            throw std::runtime_error(
                "Method " + node.methodName->name + " is of type " + expectedReturnType +
                " but is being assigned to type " + returnVariableType +
                " at line: " + std::to_string(span.get_line_num()) +
                " column: " + std::to_string(span.get_pos_begin())
            );
        }
    }

    if (node.returnType) node.returnType->accept(*this);
    if (node.methodName) node.methodName->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitMethodName(const MethodName& node)
{
}

void SymbolTableVisitor::visitParameters(const Parameters& node)
{
    for (auto& parameter : node.parameters)
    {
        parameter->accept(*this);
    }
}

void SymbolTableVisitor::visitParameter(const Parameter& node)
{
    if (node.className) node.className->accept(*this);
}

void SymbolTableVisitor::visitReturnType(const ReturnType& node)
{
}

void SymbolTableVisitor::visitVariableName(const VariableName& node)
{
}
