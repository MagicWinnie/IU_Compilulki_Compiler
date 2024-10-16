//
// Created by Nail Minnemullin on 08.10.2024.
//

#ifndef IU_COMPILULKI_COMPILER_NODES_H
#define IU_COMPILULKI_COMPILER_NODES_H

#include <iostream>
#include <utility>
#include "memory"
#include "vector"
#include "string"

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
    virtual ~Visitor() = default;
    virtual void visit(const Program& node) = 0;
    virtual void visit(const ProgramDeclaration& node) = 0;
    virtual void visit(const ClassName& node) = 0;
    virtual void visit(const ProgramArguments& node) = 0;
    virtual void visit(const Literals& node) = 0;
    virtual void visit(const Literal& node) = 0;
    virtual void visit(const Arguments& node) = 0;
    virtual void visit(const Expressions& node) = 0;
    virtual void visit(const Expression& node) = 0;
    virtual void visit(const Primary& node) = 0;
    virtual void visit(const CompoundExpression& node) = 0;
    virtual void visit(const ClassDeclarations& node) = 0;
    virtual void visit(const ClassDeclaration& node) = 0;
    virtual void visit(const ClassBody& node) = 0;
    virtual void visit(const Extension& node) = 0;
    virtual void visit(const Body& node) = 0;
    virtual void visit(const BodyDeclarations& node) = 0;
    virtual void visit(const BodyDeclaration& node) = 0;
    virtual void visit(const Statement& node) = 0;
    virtual void visit(const IfStatement& node) = 0;
    virtual void visit(const IfBranch& node) = 0;
    virtual void visit(const ElseBranch& node) = 0;
    virtual void visit(const WhileLoop& node) = 0;
    virtual void visit(const Assignment& node) = 0;
    virtual void visit(const MemberDeclarations& node) = 0;
    virtual void visit(const MemberDeclaration& node) = 0;
    virtual void visit(const ConstructorDeclaration& node) = 0;
    virtual void visit(const ReturnStatement& node) = 0;
    virtual void visit(const VariableDeclaration& node) = 0;
    virtual void visit(const MethodDeclaration& node) = 0;
    virtual void visit(const MethodName& node) = 0;
    virtual void visit(const Parameters& node) = 0;
    virtual void visit(const Parameter& node) = 0;
    virtual void visit(const ReturnType& node) = 0;
    virtual void visit(const VariableName& node) = 0;
};

class Entity
{
public:
    //    virtual bool validate() = 0;
    //    virtual void generate() = 0;
    virtual ~Entity() = default;
    virtual void accept(Visitor& visitor) const = 0;

    // Helper function to print indentation
    static void printIndent(const int indent)
    {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
    }
};

class Literals final : public Entity
{
public:
    std::vector<std::unique_ptr<Literal>> literals;

    explicit Literals(std::vector<std::unique_ptr<Literal>> literals)
        : literals(std::move(literals))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Literal final : public Entity
{
public:
    std::string value;

    explicit Literal(std::string value) : value(std::move(value))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};


class Arguments final : public Entity
{
public:
    std::unique_ptr<Expressions> expressions;

    explicit Arguments(std::unique_ptr<Expressions> expressions)
        : expressions(std::move(expressions))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};


class ClassName final : public Entity
{
public:
    std::string name;
    std::unique_ptr<ClassName> className;

    explicit ClassName(std::string name, std::unique_ptr<ClassName> className = nullptr) : name(std::move(name)),
        className(std::move(className))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ProgramDeclaration final : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<ProgramArguments> arguments;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};


class Program final : public Entity
{
public:
    std::unique_ptr<ProgramDeclaration> programDeclaration;
    std::unique_ptr<ClassDeclarations> classDeclarations;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ClassDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ClassDeclaration final : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<Extension> extension;
    std::unique_ptr<ClassBody> classBody;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class MemberDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<MemberDeclaration>> member_declarations;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Extension final : public Entity
{
public:
    std::unique_ptr<ClassName> className;

    explicit Extension(std::unique_ptr<ClassName> class_name)
        : className(std::move(class_name))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ClassBody final : public Entity
{
public:
    std::unique_ptr<MemberDeclarations> memberDeclarations;

    explicit ClassBody(std::unique_ptr<MemberDeclarations> member_declarations)
        : memberDeclarations(std::move(member_declarations))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Expressions final : public Entity
{
public:
    std::vector<std::unique_ptr<Expression>> expressions;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};


class Expression final : public Entity
{
public:
    std::unique_ptr<Primary> primary;
    std::unique_ptr<CompoundExpression> compoundExpression;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Primary final : public Entity
{
public:
    std::string value;
    std::unique_ptr<ClassName> class_name;

    explicit Primary(std::string value)
        : value(std::move(value))
    {
    }

    explicit Primary(std::unique_ptr<ClassName> class_name)
        : class_name(std::move(class_name))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class CompoundExpression final : public Entity
{
public:
    std::string identifier;
    std::unique_ptr<Arguments> arguments;
    std::vector<std::unique_ptr<CompoundExpression>> compoundExpressions;

    explicit CompoundExpression(std::string id, std::unique_ptr<Arguments> args = nullptr,
                                std::vector<std::unique_ptr<CompoundExpression>> compExpr = {})
        : identifier(std::move(id)), arguments(std::move(args)), compoundExpressions(std::move(compExpr))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ProgramArguments final : public Entity
{
public:
    std::unique_ptr<Literals> literals;

    explicit ProgramArguments(std::unique_ptr<Literals> literals)
        : literals(std::move(literals))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class MemberDeclaration final : public Entity
{
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration;
    std::unique_ptr<MethodDeclaration> methodDeclaration;
    std::unique_ptr<ConstructorDeclaration> constructorDeclaration;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ConstructorDeclaration final : public Entity
{
public:
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<Body> body;

    ConstructorDeclaration(std::unique_ptr<Parameters> parameters, std::unique_ptr<Body> body)
        : parameters(std::move(parameters)),
          body(std::move(body))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class VariableDeclaration final : public Entity
{
public:
    std::unique_ptr<VariableName> variable;
    std::unique_ptr<Expression> expression;

    VariableDeclaration(std::unique_ptr<VariableName> variable, std::unique_ptr<Expression> expression)
        : variable(std::move(variable)),
          expression(std::move(expression))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class MethodDeclaration final : public Entity
{
public:
    std::unique_ptr<MethodName> methodName;
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<ReturnType> returnType;
    std::unique_ptr<Body> body;

    MethodDeclaration(std::unique_ptr<MethodName>& method_name, std::unique_ptr<Parameters>& parameters,
                      std::unique_ptr<ReturnType>& return_type, std::unique_ptr<Body>& body)
        : methodName(std::move(method_name)),
          parameters(std::move(parameters)),
          returnType(std::move(return_type)),
          body(std::move(body))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Parameters final : public Entity
{
public:
    std::vector<std::unique_ptr<Parameter>> parameters;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class MethodName final : public Entity
{
public:
    std::string name;

    explicit MethodName(std::string name) : name(std::move(name))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Parameter final : public Entity
{
public:
    std::string name;
    std::unique_ptr<ClassName> className;

    Parameter(std::string name, std::unique_ptr<ClassName> class_name)
        : name(std::move(name)),
          className(std::move(class_name))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class VariableName final : public Entity
{
public:
    std::string name;

    explicit VariableName(std::string name) : name(std::move(name))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ReturnType final : public Entity
{
public:
    std::unique_ptr<ClassName> className;

    explicit ReturnType(std::unique_ptr<ClassName> class_name)
        : className(std::move(class_name))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class BodyDeclaration final : public Entity
{
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration = nullptr;
    std::unique_ptr<Statement> statement = nullptr;

    explicit BodyDeclaration(std::unique_ptr<VariableDeclaration> variable_declaration
    )
        : variableDeclaration(std::move(variable_declaration))
    {
    }

    explicit BodyDeclaration(std::unique_ptr<Statement> statement)
        : statement(std::move(statement))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Statement final : public Entity
{
public:
    std::unique_ptr<Assignment> assignment;
    std::unique_ptr<Expression> expression;
    std::unique_ptr<IfStatement> ifStatement;
    std::unique_ptr<WhileLoop> whileLoop;
    std::unique_ptr<ReturnStatement> returnStatement;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Assignment final : public Entity
{
public:
    std::unique_ptr<VariableName> variableName;
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class WhileLoop final : public Entity
{
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Body> body;

    WhileLoop(std::unique_ptr<Expression> expression, std::unique_ptr<Body> body)
        : expression(std::move(expression)),
          body(std::move(body))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class IfStatement final : public Entity
{
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<IfBranch> ifBranch;
    std::unique_ptr<ElseBranch> elseBranch;

    IfStatement(std::unique_ptr<Expression> expression, std::unique_ptr<IfBranch> if_branch,
                std::unique_ptr<ElseBranch> else_branch = nullptr)
        : expression(std::move(expression)),
          ifBranch(std::move(if_branch)),
          elseBranch(std::move(else_branch))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class IfBranch final : public Entity
{
public:
    std::unique_ptr<Body> body;

    explicit IfBranch(std::unique_ptr<Body> body)
        : body(std::move(body))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ElseBranch final : public Entity
{
public:
    std::unique_ptr<Body> body;

    explicit ElseBranch(std::unique_ptr<Body> body)
        : body(std::move(body))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ReturnStatement final : public Entity
{
public:
    std::unique_ptr<Expression> expression;

    explicit ReturnStatement(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class Body final : public Entity
{
public:
    std::unique_ptr<BodyDeclarations> bodyDeclarations;

    explicit Body(std::unique_ptr<BodyDeclarations> body_declarations)
        : bodyDeclarations(std::move(body_declarations))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class BodyDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<BodyDeclaration>> bodyDeclarations;

    explicit BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>> body_declarations)
        : bodyDeclarations(std::move(body_declarations))
    {
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};


#endif //IU_COMPILULKI_COMPILER_NODES_H
