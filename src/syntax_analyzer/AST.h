//
// Created by Nail Minnemullin on 08.10.2024.
//

#ifndef IU_COMPILULKI_COMPILER_NODES_H
#define IU_COMPILULKI_COMPILER_NODES_H

#include "memory"
#include "vector"
#include "string"

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

class Entity
{
public:
    //    virtual bool validate() = 0;
    //    virtual void generate() = 0;
    virtual ~Entity() = default;
};

class Literals : public Entity
{
public:
    std::vector<std::unique_ptr<Literal>> literals;

    Literals(std::vector<std::unique_ptr<Literal>> literals)
        : literals(std::move(literals))
    {
    }
};

class Literal : public Entity
{
public:
    std::string value;

    Literal(std::string value) : value(std::move(value))
    {
    }
};


class Arguments : public Entity
{
public:
    std::unique_ptr<Expressions> expressions;

    explicit Arguments(std::unique_ptr<Expressions> expressions)
        : expressions(std::move(expressions))
    {
    }
};


class ClassName : public Entity
{
public:
    std::string name;

    ClassName(std::string name) : name(std::move(name))
    {
    }
};

class ProgramDeclaration : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<ProgramArguments> arguments;
};


class Program : public Entity
{
public:
    std::unique_ptr<ProgramDeclaration> programDeclaration;
    std::unique_ptr<ClassDeclarations> classDeclarations;
};

class ClassDeclarations : public Entity
{
public:
    std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;

    explicit ClassDeclarations(std::vector<std::unique_ptr<ClassDeclaration>> class_declarations)
        : classDeclarations(std::move(class_declarations))
    {
    }
};

class ClassDeclaration : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<Extension> extension;
    std::unique_ptr<ClassBody> classBody;
};

class MemberDeclarations : public Entity
{
public:
    std::vector<std::unique_ptr<MemberDeclaration>> member_declarations;
};

class Extension : public Entity
{
public:
    std::unique_ptr<ClassName> className;

    Extension(std::unique_ptr<ClassName> class_name)
        : className(std::move(class_name))
    {
    }
};

class ClassBody : public Entity
{
public:
    std::unique_ptr<MemberDeclarations> memberDeclarations;

    explicit ClassBody(std::unique_ptr<MemberDeclarations> member_declarations)
        : memberDeclarations(std::move(member_declarations))
    {
    }
};

class Expressions : public Entity
{
public:
    std::vector<std::unique_ptr<Expression>> expressions;
};


class Expression : public Entity
{
public:
    std::unique_ptr<Primary> primary;
    std::unique_ptr<CompoundExpression> compoundExpression;
};

class Primary
{
public:
    std::string value;
    std::unique_ptr<ClassName> class_name;

    Primary(const std::string& value)
        : value(value)
    {
    }

    Primary(std::unique_ptr<ClassName> class_name)
        : class_name(std::move(class_name))
    {
    }
};

class CompoundExpression
{
public:
    std::string identifier;
    std::unique_ptr<Arguments> arguments;
    std::unique_ptr<CompoundExpression> compoundExpression;

    CompoundExpression(std::string id, std::unique_ptr<Arguments> args = nullptr,
                       std::unique_ptr<CompoundExpression> compExpr = nullptr)
        : identifier(std::move(id)), arguments(std::move(args)), compoundExpression(std::move(compExpr))
    {
    }
};

class ProgramArguments : public Entity
{
public:
    std::unique_ptr<Literals> literals;

    ProgramArguments(std::unique_ptr<Literals> literals)
        : literals(std::move(literals))
    {
    }
};

class MemberDeclaration : public Entity
{
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration;
    std::unique_ptr<MethodDeclaration> methodDeclaration;
    std::unique_ptr<ConstructorDeclaration> constructorDeclaration;
};

class ConstructorDeclaration : public Entity
{
public:
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<Body> body;

    ConstructorDeclaration(std::unique_ptr<Parameters> parameters, std::unique_ptr<Body> body)
        : parameters(std::move(parameters)),
          body(std::move(body))
    {
    }
};

class VariableDeclaration : public Entity
{
public:
    std::unique_ptr<VariableName> variable;
    std::unique_ptr<Expression> expression;

    VariableDeclaration(std::unique_ptr<VariableName> variable, std::unique_ptr<Expression> expression)
        : variable(std::move(variable)),
          expression(std::move(expression))
    {
    }
};

class MethodDeclaration : public Entity
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
};

class Parameters : public Entity
{
public:
    std::vector<std::unique_ptr<Parameter>> parameters;
};

class MethodName : public Entity
{
public:
    std::string name;

    MethodName(std::string name) : name(std::move(name))
    {
    }
};

class Parameter : public Entity
{
public:
    std::string name;
    std::unique_ptr<ClassName> className;

    Parameter(const std::string& name, std::unique_ptr<ClassName> class_name)
        : name(name),
          className(std::move(class_name))
    {
    }
};

class VariableName : public Entity
{
public:
    std::string name;

    VariableName(std::string name) : name(std::move(name))
    {
    }
};

class ReturnType : public Entity
{
public:
    std::unique_ptr<ClassName> className;

    ReturnType(std::unique_ptr<ClassName> class_name)
        : className(std::move(class_name))
    {
    }
};

class BodyDeclaration : public Entity
{
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration = nullptr;
    std::unique_ptr<Statement> statement = nullptr;

    BodyDeclaration(std::unique_ptr<VariableDeclaration> variable_declaration
    )
        : variableDeclaration(std::move(variable_declaration))
    {
    }

    explicit BodyDeclaration(std::unique_ptr<Statement> statement)
        : statement(std::move(statement))
    {
    }
};

class Statement : public Entity
{
public:
    std::unique_ptr<Assignment> assignment;
    std::unique_ptr<IfStatement> ifStatement;
    std::unique_ptr<WhileLoop> whileLoop;
    std::unique_ptr<ReturnStatement> returnStatement;
};

class Assignment : public Entity
{
public:
    std::unique_ptr<VariableName> variableName;
    std::unique_ptr<Expression> expression;
};

class WhileLoop : public Entity
{
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Body> body;

    WhileLoop(std::unique_ptr<Expression> expression, std::unique_ptr<Body> body)
        : expression(std::move(expression)),
          body(std::move(body))
    {
    }
};

class IfStatement : public Entity
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
};

class IfBranch : public Entity
{
public:
    std::unique_ptr<Body> body;

    explicit IfBranch(std::unique_ptr<Body> body)
        : body(std::move(body))
    {
    }
};

class ElseBranch : public Entity
{
public:
    std::unique_ptr<Body> body;

    explicit ElseBranch(std::unique_ptr<Body> body)
        : body(std::move(body))
    {
    }
};

class ReturnStatement : public Entity
{
public:
    std::unique_ptr<Expression> expression;
};

class Body : public Entity
{
public:
    std::unique_ptr<BodyDeclarations> bodyDeclarations;

    explicit Body(std::unique_ptr<BodyDeclarations> body_declarations)
        : bodyDeclarations(std::move(body_declarations))
    {
    }
};

class BodyDeclarations : public Entity
{
public:
    std::vector<std::unique_ptr<BodyDeclaration>> bodyDeclarations;

    explicit BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>> body_declarations)
        : bodyDeclarations(std::move(body_declarations))
    {
    }
};


#endif //IU_COMPILULKI_COMPILER_NODES_H
