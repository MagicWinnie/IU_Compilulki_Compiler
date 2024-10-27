//
// Created by Nail Minnemullin on 08.10.2024.
//
#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../lexical_analyzer/span.h"

class MemberDeclarations;
class BodyDeclaration;
class Arguments;
class Literals;
class ClassName;
class ProgramDeclaration;
class Program;
class ProgramArguments;
class ClassDeclaration;
class ClassDeclarations;
class Primary;
class CompoundExpression;
class Expressions;
class Expression;
class Parameters;
class Extension;
class BodyDeclarations;
class ConstructorDeclaration;
class Body;
class Parameter;
class ReturnType;
class ElseBranch;
class IfBranch;
class MethodDeclaration;
class VariableDeclaration;
class MethodName;
class VariableName;
class ClassBody;
class MemberDeclaration;
class ReturnStatement;
class WhileLoop;
class IfStatement;
class Assignment;
class Statement;
class Literal;

class Visitor
{
public:
    Visitor() = default;
    virtual ~Visitor() = default;
    virtual void visitProgram(const Program&) = 0;
    virtual void visitProgramDeclaration(const ProgramDeclaration&) = 0;
    virtual void visitClassName(const ClassName&) = 0;
    virtual void visitProgramArguments(const ProgramArguments&) = 0;
    virtual void visitLiterals(const Literals&) = 0;
    virtual void visitLiteral(const Literal&) = 0;
    virtual void visitArguments(const Arguments&) = 0;
    virtual void visitExpressions(const Expressions&) = 0;
    virtual void visitExpression(const Expression&) = 0;
    virtual void visitPrimary(const Primary&) = 0;
    virtual void visitCompoundExpression(const CompoundExpression&) = 0;
    virtual void visitClassDeclarations(const ClassDeclarations&) = 0;
    virtual void visitClassDeclaration(const ClassDeclaration&) = 0;
    virtual void visitClassBody(const ClassBody&) = 0;
    virtual void visitExtension(const Extension&) = 0;
    virtual void visitBody(const Body&) = 0;
    virtual void visitBodyDeclarations(const BodyDeclarations&) = 0;
    virtual void visitBodyDeclaration(const BodyDeclaration&) = 0;
    virtual void visitStatement(const Statement&) = 0;
    virtual void visitIfStatement(const IfStatement&) = 0;
    virtual void visitIfBranch(const IfBranch&) = 0;
    virtual void visitElseBranch(const ElseBranch&) = 0;
    virtual void visitWhileLoop(const WhileLoop&) = 0;
    virtual void visitAssignment(const Assignment&) = 0;
    virtual void visitMemberDeclarations(const MemberDeclarations&) = 0;
    virtual void visitMemberDeclaration(const MemberDeclaration&) = 0;
    virtual void visitConstructorDeclaration(const ConstructorDeclaration&) = 0;
    virtual void visitReturnStatement(const ReturnStatement&) = 0;
    virtual void visitVariableDeclaration(const VariableDeclaration&) = 0;
    virtual void visitMethodDeclaration(const MethodDeclaration&) = 0;
    virtual void visitMethodName(const MethodName&) = 0;
    virtual void visitParameters(const Parameters&) = 0;
    virtual void visitParameter(const Parameter&) = 0;
    virtual void visitReturnType(const ReturnType&) = 0;
    virtual void visitVariableName(const VariableName&) = 0;
};

class Entity
{
public:
    //    virtual bool validate() = 0;
    //    virtual void generate() = 0;
    virtual ~Entity();
    virtual void accept(Visitor&) const = 0;

    // Helper function to print indentation
    static void printIndent(int);
};

class Literals final : public Entity
{
public:
    std::vector<std::unique_ptr<Literal>> literals;

    explicit Literals(std::vector<std::unique_ptr<Literal>>);

    void accept(Visitor&) const override;
};

class Literal final : public Entity
{
public:
    std::string value;

    explicit Literal(std::string);

    void accept(Visitor&) const override;
};


class Arguments final : public Entity
{
public:
    std::unique_ptr<Expressions> expressions;

    explicit Arguments(std::unique_ptr<Expressions>);

    void accept(Visitor&) const override;
};


class ClassName final : public Entity
{
public:
    std::string name;
    Span span;
    std::unique_ptr<ClassName> className;

    explicit ClassName(std::string, const Span&, std::unique_ptr<ClassName> className = nullptr);

    void accept(Visitor&) const override;
};

class ProgramDeclaration final : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<ProgramArguments> arguments;

    void accept(Visitor&) const override;
};


class Program final : public Entity
{
public:
    std::unique_ptr<ProgramDeclaration> programDeclaration;
    std::unique_ptr<ClassDeclarations> classDeclarations;

    void accept(Visitor&) const override;
};

class ClassDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;

    void accept(Visitor&) const override;
};

class ClassDeclaration final : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<Extension> extension;
    std::unique_ptr<ClassBody> classBody;

    void accept(Visitor&) const override;
};

class MemberDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<MemberDeclaration>> member_declarations;

    void accept(Visitor&) const override;
};

class Extension final : public Entity
{
public:
    std::unique_ptr<ClassName> className;

    explicit Extension(std::unique_ptr<ClassName>);

    void accept(Visitor&) const override;
};

class ClassBody final : public Entity
{
public:
    std::unique_ptr<MemberDeclarations> memberDeclarations;

    explicit ClassBody(std::unique_ptr<MemberDeclarations>);

    void accept(Visitor&) const override;
};

class Expressions final : public Entity
{
public:
    std::vector<std::unique_ptr<Expression>> expressions;

    void accept(Visitor&) const override;
};


class Expression final : public Entity
{
public:
    std::unique_ptr<Primary> primary;
    std::unique_ptr<CompoundExpression> compoundExpression;

    void accept(Visitor&) const override;
};

class Primary final : public Entity
{
public:
    std::string value;
    std::unique_ptr<ClassName> class_name;

    explicit Primary(std::string);

    explicit Primary(std::unique_ptr<ClassName>);

    void accept(Visitor&) const override;
};

class CompoundExpression final : public Entity
{
public:
    std::string identifier;
    Span span;
    std::unique_ptr<Arguments> arguments;
    std::vector<std::unique_ptr<CompoundExpression>> compoundExpressions;

    explicit CompoundExpression(std::string, const Span&, std::unique_ptr<Arguments> args = nullptr,
                                std::vector<std::unique_ptr<CompoundExpression>> compExpr = {});

    void accept(Visitor&) const override;
};

class ProgramArguments final : public Entity
{
public:
    std::unique_ptr<Literals> literals;

    explicit ProgramArguments(std::unique_ptr<Literals>);

    void accept(Visitor&) const override;
};

class MemberDeclaration final : public Entity
{
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration;
    std::unique_ptr<MethodDeclaration> methodDeclaration;
    std::unique_ptr<ConstructorDeclaration> constructorDeclaration;

    void accept(Visitor&) const override;
};

class ConstructorDeclaration final : public Entity
{
public:
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<Body> body;
    Span span;

    ConstructorDeclaration(std::unique_ptr<Parameters>, std::unique_ptr<Body>, const Span&);

    void accept(Visitor&) const override;
};

class VariableDeclaration final : public Entity
{
public:
    std::unique_ptr<VariableName> variable;
    std::unique_ptr<Expression> expression;

    VariableDeclaration(std::unique_ptr<VariableName>, std::unique_ptr<Expression>);

    void accept(Visitor&) const override;
};

class MethodDeclaration final : public Entity
{
public:
    std::unique_ptr<MethodName> methodName;
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<ReturnType> returnType;
    std::unique_ptr<Body> body;

    MethodDeclaration(std::unique_ptr<MethodName>&, std::unique_ptr<Parameters>&,
                      std::unique_ptr<ReturnType>&, std::unique_ptr<Body>&);

    void accept(Visitor&) const override;
};

class Parameters final : public Entity
{
public:
    std::vector<std::unique_ptr<Parameter>> parameters;

    void accept(Visitor&) const override;
};

class MethodName final : public Entity
{
public:
    std::string name;
    Span span;

    explicit MethodName(std::string, Span);

    void accept(Visitor&) const override;
};

class Parameter final : public Entity
{
public:
    std::string name;
    Span span;
    std::unique_ptr<ClassName> className;

    Parameter(std::string, const Span&, std::unique_ptr<ClassName>);

    void accept(Visitor&) const override;
};

class VariableName final : public Entity
{
public:
    std::string name;
    Span span;

    explicit VariableName(std::string, const Span&);

    void accept(Visitor&) const override;
};

class ReturnType final : public Entity
{
public:
    std::unique_ptr<ClassName> className;

    explicit ReturnType(std::unique_ptr<ClassName>);

    void accept(Visitor&) const override;
};

class BodyDeclaration final : public Entity
{
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration = nullptr;
    std::unique_ptr<Statement> statement = nullptr;

    explicit BodyDeclaration(std::unique_ptr<VariableDeclaration>);

    explicit BodyDeclaration(std::unique_ptr<Statement>);

    void accept(Visitor&) const override;
};

class Statement final : public Entity
{
public:
    std::unique_ptr<Assignment> assignment;
    std::unique_ptr<Expression> expression;
    std::unique_ptr<IfStatement> ifStatement;
    std::unique_ptr<WhileLoop> whileLoop;
    std::unique_ptr<ReturnStatement> returnStatement;

    void accept(Visitor&) const override;
};

class Assignment final : public Entity
{
public:
    std::unique_ptr<VariableName> variableName;
    std::unique_ptr<Expression> expression;

    void accept(Visitor&) const override;
};

class WhileLoop final : public Entity
{
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Body> body;

    WhileLoop(std::unique_ptr<Expression>, std::unique_ptr<Body>);

    void accept(Visitor&) const override;
};

class IfStatement final : public Entity
{
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<IfBranch> ifBranch;
    std::unique_ptr<ElseBranch> elseBranch;

    IfStatement(std::unique_ptr<Expression>, std::unique_ptr<IfBranch>,
                std::unique_ptr<ElseBranch> else_branch = nullptr);

    void accept(Visitor&) const override;
};

class IfBranch final : public Entity
{
public:
    std::unique_ptr<Body> body;

    explicit IfBranch(std::unique_ptr<Body>);

    void accept(Visitor&) const override;
};

class ElseBranch final : public Entity
{
public:
    std::unique_ptr<Body> body;

    explicit ElseBranch(std::unique_ptr<Body>);

    void accept(Visitor&) const override;
};

class ReturnStatement final : public Entity
{
public:
    std::unique_ptr<Expression> expression;

    explicit ReturnStatement(std::unique_ptr<Expression>);

    void accept(Visitor&) const override;
};

class Body final : public Entity
{
public:
    std::unique_ptr<BodyDeclarations> bodyDeclarations;

    explicit Body(std::unique_ptr<BodyDeclarations>);

    void accept(Visitor&) const override;
};

class BodyDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<BodyDeclaration>> bodyDeclarations;

    explicit BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>>);

    void accept(Visitor&) const override;
};
