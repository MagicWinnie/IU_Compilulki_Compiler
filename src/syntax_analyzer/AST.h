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
    Visitor() = default;
    virtual ~Visitor() = default;
    virtual void visitProgram(const Program& node) = 0;
    virtual void visitProgramDeclaration(const ProgramDeclaration& node) = 0;
    virtual void visitClassName(const ClassName& node) = 0;
    virtual void visitProgramArguments(const ProgramArguments& node) = 0;
    virtual void visitLiterals(const Literals& node) = 0;
    virtual void visitLiteral(const Literal& node) = 0;
    virtual void visitArguments(const Arguments& node) = 0;
    virtual void visitExpressions(const Expressions& node) = 0;
    virtual void visitExpression(const Expression& node) = 0;
    virtual void visitPrimary(const Primary& node) = 0;
    virtual void visitCompoundExpression(const CompoundExpression& node) = 0;
    virtual void visitClassDeclarations(const ClassDeclarations& node) = 0;
    virtual void visitClassDeclaration(const ClassDeclaration& node) = 0;
    virtual void visitClassBody(const ClassBody& node) = 0;
    virtual void visitExtension(const Extension& node) = 0;
    virtual void visitBody(const Body& node) = 0;
    virtual void visitBodyDeclarations(const BodyDeclarations& node) = 0;
    virtual void visitBodyDeclaration(const BodyDeclaration& node) = 0;
    virtual void visitStatement(const Statement& node) = 0;
    virtual void visitIfStatement(const IfStatement& node) = 0;
    virtual void visitIfBranch(const IfBranch& node) = 0;
    virtual void visitElseBranch(const ElseBranch& node) = 0;
    virtual void visitWhileLoop(const WhileLoop& node) = 0;
    virtual void visitAssignment(const Assignment& node) = 0;
    virtual void visitMemberDeclarations(const MemberDeclarations& node) = 0;
    virtual void visitMemberDeclaration(const MemberDeclaration& node) = 0;
    virtual void visitConstructorDeclaration(const ConstructorDeclaration& node) = 0;
    virtual void visitReturnStatement(const ReturnStatement& node) = 0;
    virtual void visitVariableDeclaration(const VariableDeclaration& node) = 0;
    virtual void visitMethodDeclaration(const MethodDeclaration& node) = 0;
    virtual void visitMethodName(const MethodName& node) = 0;
    virtual void visitParameters(const Parameters& node) = 0;
    virtual void visitParameter(const Parameter& node) = 0;
    virtual void visitReturnType(const ReturnType& node) = 0;
    virtual void visitVariableName(const VariableName& node) = 0;
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
        visitor.visitLiterals(*this);
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
        visitor.visitLiteral(*this);
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
        visitor.visitArguments(*this);
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
        visitor.visitClassName(*this);
    }
};

class ProgramDeclaration final : public Entity
{
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<ProgramArguments> arguments;

    void accept(Visitor& visitor) const override
    {
        visitor.visitProgramDeclaration(*this);
    }
};


class Program final : public Entity
{
public:
    std::unique_ptr<ProgramDeclaration> programDeclaration;
    std::unique_ptr<ClassDeclarations> classDeclarations;

    void accept(Visitor& visitor) const override
    {
        visitor.visitProgram(*this);
    }
};

class ClassDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;

    void accept(Visitor& visitor) const override
    {
        visitor.visitClassDeclarations(*this);
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
        visitor.visitClassDeclaration(*this);
    }
};

class MemberDeclarations final : public Entity
{
public:
    std::vector<std::unique_ptr<MemberDeclaration>> member_declarations;

    void accept(Visitor& visitor) const override
    {
        visitor.visitMemberDeclarations(*this);
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
        visitor.visitExtension(*this);
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
        visitor.visitClassBody(*this);
    }
};

class Expressions final : public Entity
{
public:
    std::vector<std::unique_ptr<Expression>> expressions;

    void accept(Visitor& visitor) const override
    {
        visitor.visitExpressions(*this);
    }
};


class Expression final : public Entity
{
public:
    std::unique_ptr<Primary> primary;
    std::unique_ptr<CompoundExpression> compoundExpression;

    void accept(Visitor& visitor) const override
    {
        visitor.visitExpression(*this);
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
        visitor.visitPrimary(*this);
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
        visitor.visitCompoundExpression(*this);
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
        visitor.visitProgramArguments(*this);
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
        visitor.visitMemberDeclaration(*this);
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
        visitor.visitConstructorDeclaration(*this);
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
        visitor.visitVariableDeclaration(*this);
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
        visitor.visitMethodDeclaration(*this);
    }
};

class Parameters final : public Entity
{
public:
    std::vector<std::unique_ptr<Parameter>> parameters;

    void accept(Visitor& visitor) const override
    {
        visitor.visitParameters(*this);
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
        visitor.visitMethodName(*this);
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
        visitor.visitParameter(*this);
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
        visitor.visitVariableName(*this);
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
        visitor.visitReturnType(*this);
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
        visitor.visitBodyDeclaration(*this);
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
        visitor.visitStatement(*this);
    }
};

class Assignment final : public Entity
{
public:
    std::unique_ptr<VariableName> variableName;
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) const override
    {
        visitor.visitAssignment(*this);
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
        visitor.visitWhileLoop(*this);
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
        visitor.visitIfStatement(*this);
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
        visitor.visitIfBranch(*this);
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
        visitor.visitElseBranch(*this);
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
        visitor.visitReturnStatement(*this);
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
        visitor.visitBody(*this);
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
        visitor.visitBodyDeclarations(*this);
    }
};


#endif //IU_COMPILULKI_COMPILER_NODES_H
