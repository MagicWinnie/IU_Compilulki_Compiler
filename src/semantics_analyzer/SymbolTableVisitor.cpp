#include <iostream>
#include <stdexcept>
#include "SymbolTableVisitor.h"

SymbolTableVisitor::SymbolTableVisitor()
{
    symbolTable.enterScope();
    // TODO: temporary not correct fix for current state
    symbolTable.addClassEntry("Integer", Span(0, 0, 0));
    symbolTable.addClassEntry("Real", Span(0, 0, 0));
    symbolTable.addClassEntry("Array", Span(0, 0, 0));
    symbolTable.addClassEntry("List", Span(0, 0, 0));
    symbolTable.addClassEntry("String", Span(0, 0, 0));
}

SymbolTableVisitor::~SymbolTableVisitor()
{
    symbolTable.leaveScope();
}

ScopedSymbolTable SymbolTableVisitor::getSymbolTable() const
{
    return symbolTable;
}

void SymbolTableVisitor::visitProgram(Program& node)
{
    if (node.programDeclaration) node.programDeclaration->accept(*this);
    if (node.classDeclarations) node.classDeclarations->accept(*this);
    if (node.programDeclaration && node.programDeclaration->className)
        node.programDeclaration->className->accept(*this);
}

void SymbolTableVisitor::visitProgramDeclaration(ProgramDeclaration& node)
{
    // if (node.className) node.className->accept(*this);
    if (node.arguments) node.arguments->accept(*this);
}

void SymbolTableVisitor::visitClassName(ClassName& node)
{
    symbolTable.lookupClass(node.name, node.span);
}

void SymbolTableVisitor::visitProgramArguments(ProgramArguments& node)
{
    if (node.literals) node.literals->accept(*this);
}

void SymbolTableVisitor::visitLiterals(Literals& node)
{
    for (const auto& literal : node.literals)
    {
        literal->accept(*this);
    }
}

void SymbolTableVisitor::visitLiteral(Literal& node)
{
}

void SymbolTableVisitor::visitArguments(Arguments& node)
{
    if (node.expressions) node.expressions->accept(*this);
}

void SymbolTableVisitor::visitExpressions(Expressions& node)
{
    for (const auto& expression : node.expressions)
    {
        expression->accept(*this);
    }
}

void SymbolTableVisitor::visitExpression(Expression& node)
{
    if (node.primary) node.primary->accept(*this);
    if (node.compoundExpression) node.compoundExpression->accept(*this);
}

void SymbolTableVisitor::visitPrimary(Primary& node)
{
    if (node.class_name) node.class_name->accept(*this);
}

void SymbolTableVisitor::visitCompoundExpression(CompoundExpression& node)
{
    // TODO check if identifier is a variable or a class or a method

    if (symbolTable.lookupVariable(node.identifier, node.span, false) != nullptr)
    {
        symbolTable.makeVariableUsed(node.identifier);
    }

    if (node.arguments) node.arguments->accept(*this);
    for (const auto& compoundExpression : node.compoundExpressions)
    {
        compoundExpression->accept(*this);
    }
}

void SymbolTableVisitor::visitClassDeclarations(ClassDeclarations& node)
{
    for (const auto& classDeclaration : node.classDeclarations)
    {
        symbolTable.addClassEntry(classDeclaration->className->name, classDeclaration->className->span);
        classDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitClassDeclaration(ClassDeclaration& node)
{
    symbolTable.enterScope();
    if (node.className) node.className->accept(*this);
    if (node.extension) node.extension->accept(*this);
    if (node.classBody) node.classBody->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitClassBody(ClassBody& node)
{
    if (node.memberDeclarations) node.memberDeclarations->accept(*this);
}

void SymbolTableVisitor::visitExtension(Extension& node)
{
    if (node.className) node.className->accept(*this);
}

void SymbolTableVisitor::visitBody(Body& node)
{
    if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
}

void SymbolTableVisitor::visitBodyDeclarations(BodyDeclarations& node)
{
    for (auto& bodyDeclaration : node.bodyDeclarations)
    {
        if (bodyDeclaration) bodyDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitBodyDeclaration(BodyDeclaration& node)
{
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    if (node.statement) node.statement->accept(*this);
}

void SymbolTableVisitor::visitStatement(Statement& node)
{
    node.accept(*this);
}

void SymbolTableVisitor::visitIfStatement(IfStatement& node)
{
    if (node.expression) node.expression->accept(*this);
    if (node.ifBranch) node.ifBranch->accept(*this);
    if (node.elseBranch) node.elseBranch->accept(*this);
}

void SymbolTableVisitor::visitIfBranch(IfBranch& node)
{
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitElseBranch(ElseBranch& node)
{
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitWhileLoop(WhileLoop& node)
{
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    if (node.expression) node.expression->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitAssignment(Assignment& node)
{
    // [CHECK] if variable is declared
    try
    {
        symbolTable.lookupVariable(node.variableName->name, node.variableName->span);
        symbolTable.makeVariableUsed(node.variableName->name);
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

void SymbolTableVisitor::visitMemberDeclarations(MemberDeclarations& node)
{
    for (const auto& memberDeclaration : node.member_declarations)
    {
        memberDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitMemberDeclaration(MemberDeclaration& node)
{
    if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
    if (node.methodDeclaration) node.methodDeclaration->accept(*this);
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
}

void SymbolTableVisitor::visitConstructorDeclaration(ConstructorDeclaration& node)
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

    const ReturnStatement* returnStatement = nullptr;
    for (const auto& bodyDeclaration : node.body->bodyDeclarations->bodyDeclarations)
    {
        if (bodyDeclaration && bodyDeclaration->statement && bodyDeclaration->statement->type ==
            RETURN_STATEMENT)
        {
            // dynamic cast statement to ReturnStatement
            returnStatement = dynamic_cast<ReturnStatement*>(bodyDeclaration->statement.get());
        }
    }


    // [CHECK] if constructor has return statement
    if (returnStatement != nullptr)
    {
        throw std::runtime_error("Constructor cannot have return statement");
    }

    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitReturnStatement(ReturnStatement& node)
{
    if (node.expression) node.expression->accept(*this);
}

void SymbolTableVisitor::visitVariableDeclaration(VariableDeclaration& node)
{
    if (node.expression->primary)
    {
        //symbolTable.addVariableEntry(node.variable->name, node.expression->);
    }
    else if (node.expression->compoundExpression)
    {
        if (node.expression->compoundExpression->compoundExpressions.empty())
        {
            symbolTable.lookupClass(node.expression->compoundExpression->identifier,
                                    node.expression->compoundExpression->span);
        }
        symbolTable.addVariableEntry(node.variable->name, node.expression->compoundExpression->identifier,
                                     node.variable->span);
    }

    if (node.expression) node.expression->accept(*this);
    if (node.variable) node.variable->accept(*this);
}

void SymbolTableVisitor::visitMethodDeclaration(MethodDeclaration& node)
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
    const auto expectedReturnType = symbolTable.lookupFunction(node.methodName->name, node.methodName->span)->
                                                returnType;
    const ReturnStatement* returnStatement = nullptr;
    for (const auto& bodyDeclaration : node.body->bodyDeclarations->bodyDeclarations)
    {
        if (bodyDeclaration && bodyDeclaration->statement && bodyDeclaration->statement->type == RETURN_STATEMENT)
        {
            returnStatement = dynamic_cast<ReturnStatement*>(bodyDeclaration->statement.get());
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
        const auto returnVariable = symbolTable.lookupVariable(
            returnStatement->expression->compoundExpression->identifier,
            span
        );
        if (expectedReturnType != returnVariable->type)
        {
            throw std::runtime_error(
                "Method " + node.methodName->name + " is of type " + expectedReturnType +
                " but is being assigned to type " + returnVariable->type +
                " at line: " + std::to_string(span.get_line_num()) +
                " column: " + std::to_string(span.get_pos_begin())
            );
        }
    }

    if (node.returnType) node.returnType->accept(*this);
    if (node.methodName) node.methodName->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitMethodName(MethodName& node)
{
}

void SymbolTableVisitor::visitParameters(Parameters& node)
{
    for (const auto& parameter : node.parameters)
    {
        parameter->accept(*this);
    }
}

void SymbolTableVisitor::visitParameter(Parameter& node)
{
    if (node.className) node.className->accept(*this);
}

void SymbolTableVisitor::visitReturnType(ReturnType& node)
{
}

void SymbolTableVisitor::visitVariableName(VariableName& node)
{
}
