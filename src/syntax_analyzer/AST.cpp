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

void Literals::accept(Visitor& visitor) const
{
    visitor.visitLiterals(*this);
}

Literal::Literal(std::string value): value(std::move(value))
{
}

void Literal::accept(Visitor& visitor) const
{
    visitor.visitLiteral(*this);
}

Arguments::Arguments(std::unique_ptr<Expressions> expressions): expressions(std::move(expressions))
{
}

void Arguments::accept(Visitor& visitor) const
{
    visitor.visitArguments(*this);
}

ClassName::ClassName(std::string name, std::unique_ptr<ClassName> className):
    name(std::move(name)), className(std::move(className))
{
}

void ClassName::accept(Visitor& visitor) const
{
    visitor.visitClassName(*this);
}

void ProgramDeclaration::accept(Visitor& visitor) const
{
    visitor.visitProgramDeclaration(*this);
}

void Program::accept(Visitor& visitor) const
{
    visitor.visitProgram(*this);
}

void ClassDeclarations::accept(Visitor& visitor) const
{
    visitor.visitClassDeclarations(*this);
}

void ClassDeclaration::accept(Visitor& visitor) const
{
    visitor.visitClassDeclaration(*this);
}

void MemberDeclarations::accept(Visitor& visitor) const
{
    visitor.visitMemberDeclarations(*this);
}

Extension::Extension(std::unique_ptr<ClassName> class_name): className(std::move(class_name))
{
}

void Extension::accept(Visitor& visitor) const
{
    visitor.visitExtension(*this);
}

ClassBody::ClassBody(std::unique_ptr<MemberDeclarations> member_declarations): memberDeclarations(
    std::move(member_declarations))
{
}

void ClassBody::accept(Visitor& visitor) const
{
    visitor.visitClassBody(*this);
}

void Expressions::accept(Visitor& visitor) const
{
    visitor.visitExpressions(*this);
}

void Expression::accept(Visitor& visitor) const
{
    visitor.visitExpression(*this);
}

Primary::Primary(std::string value): value(std::move(value))
{
}

Primary::Primary(std::unique_ptr<ClassName> class_name): class_name(std::move(class_name))
{
}

void Primary::accept(Visitor& visitor) const
{
    visitor.visitPrimary(*this);
}

CompoundExpression::CompoundExpression(std::string id, const Span& span, std::unique_ptr<Arguments> args,
                                       std::vector<std::unique_ptr<CompoundExpression>> compExpr):
    identifier(std::move(id)), span(span), arguments(std::move(args)), compoundExpressions(std::move(compExpr))
{
}

void CompoundExpression::accept(Visitor& visitor) const
{
    visitor.visitCompoundExpression(*this);
}

ProgramArguments::ProgramArguments(std::unique_ptr<Literals> literals): literals(std::move(literals))
{
}

void ProgramArguments::accept(Visitor& visitor) const
{
    visitor.visitProgramArguments(*this);
}

void MemberDeclaration::accept(Visitor& visitor) const
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

void ConstructorDeclaration::accept(Visitor& visitor) const
{
    visitor.visitConstructorDeclaration(*this);
}

VariableDeclaration::VariableDeclaration(std::unique_ptr<VariableName> variable,
                                         std::unique_ptr<Expression> expression): variable(std::move(variable)),
    expression(std::move(expression))
{
}

void VariableDeclaration::accept(Visitor& visitor) const
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

void MethodDeclaration::accept(Visitor& visitor) const
{
    visitor.visitMethodDeclaration(*this);
}

void Parameters::accept(Visitor& visitor) const
{
    visitor.visitParameters(*this);
}

MethodName::MethodName(std::string name, Span span): name(std::move(name)), span(span)
{
}

void MethodName::accept(Visitor& visitor) const
{
    visitor.visitMethodName(*this);
}

Parameter::Parameter(std::string name, const Span& span, std::unique_ptr<ClassName> class_name):
    name(std::move(name)), span(span), className(std::move(class_name))
{
}

void Parameter::accept(Visitor& visitor) const
{
    visitor.visitParameter(*this);
}

VariableName::VariableName(std::string name, const Span& span): name(std::move(name)), span(span)
{
}

void VariableName::accept(Visitor& visitor) const
{
    visitor.visitVariableName(*this);
}

ReturnType::ReturnType(std::unique_ptr<ClassName> class_name): className(std::move(class_name))
{
}

void ReturnType::accept(Visitor& visitor) const
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

void BodyDeclaration::accept(Visitor& visitor) const
{
    visitor.visitBodyDeclaration(*this);
}

void Statement::accept(Visitor& visitor) const
{
    visitor.visitStatement(*this);
}

void Assignment::accept(Visitor& visitor) const
{
    visitor.visitAssignment(*this);
}

WhileLoop::WhileLoop(std::unique_ptr<Expression> expression, std::unique_ptr<Body> body):
    expression(std::move(expression)),
    body(std::move(body))
{
}

void WhileLoop::accept(Visitor& visitor) const
{
    visitor.visitWhileLoop(*this);
}

IfStatement::IfStatement(std::unique_ptr<Expression> expression, std::unique_ptr<IfBranch> if_branch,
                         std::unique_ptr<ElseBranch> else_branch): expression(std::move(expression)),
                                                                   ifBranch(std::move(if_branch)),
                                                                   elseBranch(std::move(else_branch))
{
}

void IfStatement::accept(Visitor& visitor) const
{
    visitor.visitIfStatement(*this);
}

IfBranch::IfBranch(std::unique_ptr<Body> body): body(std::move(body))
{
}

void IfBranch::accept(Visitor& visitor) const
{
    visitor.visitIfBranch(*this);
}

ElseBranch::ElseBranch(std::unique_ptr<Body> body): body(std::move(body))
{
}

void ElseBranch::accept(Visitor& visitor) const
{
    visitor.visitElseBranch(*this);
}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> expression): expression(std::move(expression))
{
}

void ReturnStatement::accept(Visitor& visitor) const
{
    visitor.visitReturnStatement(*this);
}

Body::Body(std::unique_ptr<BodyDeclarations> body_declarations): bodyDeclarations(std::move(body_declarations))
{
}

void Body::accept(Visitor& visitor) const
{
    visitor.visitBody(*this);
}

BodyDeclarations::BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>> body_declarations): bodyDeclarations(
    std::move(body_declarations))
{
}

void BodyDeclarations::accept(Visitor& visitor) const
{
    visitor.visitBodyDeclarations(*this);
}
