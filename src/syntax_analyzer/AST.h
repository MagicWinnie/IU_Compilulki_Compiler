//
// Created by Nail Minnemullin on 08.10.2024.
//

#ifndef IU_COMPILULKI_COMPILER_NODES_H
#define IU_COMPILULKI_COMPILER_NODES_H

#include "memory"
#include "vector"
#include "string"

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
    std::vector<std::unique_ptr<Literal>> literals;

    Literals(std::vector<std::unique_ptr<Literal>> literals)
        : literals(std::move(literals))
    {
    }
};

class Literal : public Entity
{
    std::string value;

    Literal(std::string value) : value(std::move(value))
    {
    }
};


class Arguments : public Entity
{
    std::vector<std::unique_ptr<Entity>> arguments;

    Arguments(std::vector<std::unique_ptr<Entity>> args)
        : arguments(std::move(args))
    {
    }
};


class ClassName : public Entity
{
    std::string name;

    ClassName(std::string name) : name(std::move(name))
    {
    }
};

class ProgramDeclaration : public Entity
{
public:
    std::string className;
    std::unique_ptr<Arguments> arguments;
};


class Program : public Entity
{
public:
    std::unique_ptr<ProgramDeclaration> programDeclaration;
    std::vector<std::unique_ptr<Entity>> classDeclarations;
};

class ClassDeclaration : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<Extension> extension;
    std::unique_ptr<ClassBody> classBody;

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
    std::vector<std::unique_ptr<MemberDeclaration>> memberDeclarations;\

    explicit ClassBody(const std::vector<std::unique_ptr<MemberDeclaration>>& member_declarations)
        : memberDeclarations(member_declarations)
    {
    }
};

class Expression : public Entity
{
    std::unique_ptr<Primary> primary;
    std::unique_ptr<CompoundExpression> compoundExpression;
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
    std::vector<std::unique_ptr<Parameter>> parameters;
    std::unique_ptr<Body> body;
};

class VariableDeclaration : public Entity
{
public:
    std::unique_ptr<VariableName> variable;
    std::unique_ptr<Expression> expression;
};

class MethodDeclaration : public Entity
{
public:
    std::unique_ptr<MethodName> methodName;
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<ReturnType> returnType;
    std::unique_ptr<Body> body;
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

    Parameter(const std::string& name,std::unique_ptr<ClassName> class_name)
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
};

class IfStatement : public Entity
{
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<IfBranch> ifBranch;
    std::unique_ptr<ElseBranch> elseBranch;
};

class IfBranch : public Entity
{
public:
    std::unique_ptr<Body> body;
};

class ElseBranch : public Entity
{
public:
    std::unique_ptr<Body> body;
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

    explicit BodyDeclarations(const std::vector<std::unique_ptr<BodyDeclaration>>& body_declarations)
        : bodyDeclarations(body_declarations)
    {
    }
};


#endif //IU_COMPILULKI_COMPILER_NODES_H
