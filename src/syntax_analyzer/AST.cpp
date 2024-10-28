//
// Created by Nail Minnemullin on 08.10.2024.
//

#include <iostream>
#include <utility>
#include "AST.h"

Entity::~Entity() = default;

void Entity::printIndent(const int indent)
{
    for (int i = 0; i < indent; ++i) std::cout << "  ";
}

Literals::Literals(std::vector<std::unique_ptr<Literal>> literals): literals(std::move(literals))
{
}

void Literals::accept(Visitor& visitor)
{
    visitor.visitLiterals(*this);
}



void Literal::accept(Visitor& visitor)
{
    visitor.visitLiteral(*this);
}

Arguments::Arguments(std::unique_ptr<Expressions> expressions): expressions(std::move(expressions))
{
}

void Arguments::accept(Visitor& visitor)
{
    visitor.visitArguments(*this);
}

ClassName::ClassName(std::string name, const Span& span, std::unique_ptr<ClassName> className):
    name(std::move(name)), span(span), className(std::move(className))
{
}

void ClassName::accept(Visitor& visitor)
{
    visitor.visitClassName(*this);
}

void ProgramDeclaration::accept(Visitor& visitor)
{
    visitor.visitProgramDeclaration(*this);
}

void Program::accept(Visitor& visitor)
{
    visitor.visitProgram(*this);
}

void ClassDeclarations::accept(Visitor& visitor)
{
    visitor.visitClassDeclarations(*this);
}

void ClassDeclaration::accept(Visitor& visitor)
{
    visitor.visitClassDeclaration(*this);
}

void MemberDeclarations::accept(Visitor& visitor)
{
    visitor.visitMemberDeclarations(*this);
}

Extension::Extension(std::unique_ptr<ClassName> class_name): className(std::move(class_name))
{
}

void Extension::accept(Visitor& visitor)
{
    visitor.visitExtension(*this);
}

ClassBody::ClassBody(std::unique_ptr<MemberDeclarations> member_declarations): memberDeclarations(
    std::move(member_declarations))
{
}

void ClassBody::accept(Visitor& visitor)
{
    visitor.visitClassBody(*this);
}

void Expressions::accept(Visitor& visitor)
{
    visitor.visitExpressions(*this);
}

void Expression::accept(Visitor& visitor)
{
    visitor.visitExpression(*this);
}



void Primary::accept(Visitor& visitor)
{
    visitor.visitPrimary(*this);
}

CompoundExpression::CompoundExpression(std::string id, const Span& span, std::unique_ptr<Arguments> args,
                                       std::vector<std::unique_ptr<CompoundExpression>> compExpr):
    identifier(std::move(id)), span(span), arguments(std::move(args)), compoundExpressions(std::move(compExpr))
{
    type = COMPOUND_STATEMENT;
}

void CompoundExpression::accept(Visitor& visitor)
{
    visitor.visitCompoundExpression(*this);
}

ProgramArguments::ProgramArguments(std::unique_ptr<Literals> literals): literals(std::move(literals))
{
}

void ProgramArguments::accept(Visitor& visitor)
{
    visitor.visitProgramArguments(*this);
}

void MemberDeclaration::accept(Visitor& visitor)
{
    visitor.visitMemberDeclaration(*this);
}

ConstructorDeclaration::ConstructorDeclaration(std::unique_ptr<Parameters> parameters, std::unique_ptr<Body> body,
                                               const Span& span):
    parameters(std::move(parameters)),
    body(std::move(body)),
    span(span)
{
}

void ConstructorDeclaration::accept(Visitor& visitor)
{
    visitor.visitConstructorDeclaration(*this);
}

VariableDeclaration::VariableDeclaration(std::unique_ptr<VariableName> variable,
                                         std::unique_ptr<Expression> expression): variable(std::move(variable)),
    expression(std::move(expression))
{
}

void VariableDeclaration::accept(Visitor& visitor)
{
    visitor.visitVariableDeclaration(*this);
}

MethodDeclaration::MethodDeclaration(std::unique_ptr<MethodName>& method_name, std::unique_ptr<Parameters>& parameters,
                                     std::unique_ptr<ReturnType>& return_type,
                                     std::unique_ptr<Body>& body): methodName(std::move(method_name)),
                                                                   parameters(std::move(parameters)),
                                                                   returnType(std::move(return_type)),
                                                                   body(std::move(body))
{
}

void MethodDeclaration::accept(Visitor& visitor)
{
    visitor.visitMethodDeclaration(*this);
}

void Parameters::accept(Visitor& visitor)
{
    visitor.visitParameters(*this);
}

MethodName::MethodName(std::string name, const Span& span): name(std::move(name)), span(span)
{
}

void MethodName::accept(Visitor& visitor)
{
    visitor.visitMethodName(*this);
}

Parameter::Parameter(std::string name, const Span& span, std::unique_ptr<ClassName> class_name):
    name(std::move(name)), span(span), className(std::move(class_name))
{
}

void Parameter::accept(Visitor& visitor)
{
    visitor.visitParameter(*this);
}

VariableName::VariableName(std::string name, const Span& span): name(std::move(name)), span(span)
{
}

void VariableName::accept(Visitor& visitor)
{
    visitor.visitVariableName(*this);
}

ReturnType::ReturnType(std::unique_ptr<ClassName> class_name): className(std::move(class_name))
{
}

void ReturnType::accept(Visitor& visitor)
{
    visitor.visitReturnType(*this);
}

BodyDeclaration::BodyDeclaration(std::unique_ptr<VariableDeclaration> variable_declaration): variableDeclaration(
    std::move(variable_declaration))
{
}

BodyDeclaration::BodyDeclaration(std::unique_ptr<Statement> statement): statement(std::move(statement))
{
}

void BodyDeclaration::accept(Visitor& visitor)
{
    visitor.visitBodyDeclaration(*this);
}

void Statement::accept(Visitor& visitor)
{
    visitor.visitStatement(*this);
}

void Assignment::accept(Visitor& visitor)
{
    visitor.visitAssignment(*this);
}

WhileLoop::WhileLoop(std::unique_ptr<Expression> expression, std::unique_ptr<Body> body):
    expression(std::move(expression)),
    body(std::move(body))
{
    type = WHILE_LOOP;
}

void WhileLoop::accept(Visitor& visitor)
{
    visitor.visitWhileLoop(*this);
}

IfStatement::IfStatement(std::unique_ptr<Expression> expression, std::unique_ptr<IfBranch> if_branch,
                         std::unique_ptr<ElseBranch> else_branch): expression(std::move(expression)),
                                                                   ifBranch(std::move(if_branch)),
                                                                   elseBranch(std::move(else_branch))
{
    type = IF_STATEMENT;
}

void IfStatement::accept(Visitor& visitor)
{
    visitor.visitIfStatement(*this);
}

IfBranch::IfBranch(std::unique_ptr<Body> body): body(std::move(body))
{
}

void IfBranch::accept(Visitor& visitor)
{
    visitor.visitIfBranch(*this);
}

ElseBranch::ElseBranch(std::unique_ptr<Body> body): body(std::move(body))
{
}

void ElseBranch::accept(Visitor& visitor)
{
    visitor.visitElseBranch(*this);
}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> expression): expression(std::move(expression))
{
    type = RETURN_STATEMENT;
}

void ReturnStatement::accept(Visitor& visitor)
{
    visitor.visitReturnStatement(*this);
}

Body::Body(std::unique_ptr<BodyDeclarations> body_declarations): bodyDeclarations(std::move(body_declarations))
{
}

void Body::accept(Visitor& visitor)
{
    visitor.visitBody(*this);
}

BodyDeclarations::BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>> body_declarations): bodyDeclarations(
    std::move(body_declarations))
{
}

void BodyDeclarations::accept(Visitor& visitor)
{
    visitor.visitBodyDeclarations(*this);
}
