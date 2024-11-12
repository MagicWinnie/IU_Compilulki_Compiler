//
// Created by Nail Minnemullin on 08.10.2024.
//
#pragma once

#include <memory>
#include <vector>
#include <string>
#include "iostream"
#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Verifier.h"

#include "../lexical_analyzer/span.h"
#include "../semantics_analyzer/SymbolTable.h"


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

class Visitor {
public:
    Visitor() = default;

    virtual ~Visitor() = default;

    virtual void visitProgram(Program &) = 0;

    virtual void visitProgramDeclaration(ProgramDeclaration &) = 0;

    virtual void visitClassName(ClassName &) = 0;

    virtual void visitProgramArguments(ProgramArguments &) = 0;

    virtual void visitLiterals(Literals &) = 0;

    virtual void visitLiteral(Literal &) = 0;

    virtual void visitArguments(Arguments &) = 0;

    virtual void visitExpressions(Expressions &) = 0;

    virtual void visitExpression(Expression &) = 0;

    virtual void visitPrimary(Primary &) = 0;

    virtual void visitCompoundExpression(CompoundExpression &) = 0;

    virtual void visitClassDeclarations(ClassDeclarations &) = 0;

    virtual void visitClassDeclaration(ClassDeclaration &) = 0;

    virtual void visitClassBody(ClassBody &) = 0;

    virtual void visitExtension(Extension &) = 0;

    virtual void visitBody(Body &) = 0;

    virtual void visitBodyDeclarations(BodyDeclarations &) = 0;

    virtual void visitBodyDeclaration(BodyDeclaration &) = 0;

    virtual void visitStatement(Statement &) = 0;

    virtual void visitIfStatement(IfStatement &) = 0;

    virtual void visitIfBranch(IfBranch &) = 0;

    virtual void visitElseBranch(ElseBranch &) = 0;

    virtual void visitWhileLoop(WhileLoop &) = 0;

    virtual void visitAssignment(Assignment &) = 0;

    virtual void visitMemberDeclarations(MemberDeclarations &) = 0;

    virtual void visitMemberDeclaration(MemberDeclaration &) = 0;

    virtual void visitConstructorDeclaration(ConstructorDeclaration &) = 0;

    virtual void visitReturnStatement(ReturnStatement &) = 0;

    virtual void visitVariableDeclaration(VariableDeclaration &) = 0;

    virtual void visitMethodDeclaration(MethodDeclaration &) = 0;

    virtual void visitMethodName(MethodName &) = 0;

    virtual void visitParameters(Parameters &) = 0;

    virtual void visitParameter(Parameter &) = 0;

    virtual void visitReturnType(ReturnType &) = 0;

    virtual void visitVariableName(VariableName &) = 0;
};


class Entity {
public:
    //    virtual bool validate() = 0;
    //    virtual void generate() = 0;
    virtual ~Entity();

    virtual void accept(Visitor &) = 0;

    virtual llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) = 0;

    // Helper function to print indentation
    static void printIndent(int);
};

class Literals final : public Entity {
public:
    std::vector<std::unique_ptr<Literal>> literals;

    explicit Literals(std::vector<std::unique_ptr<Literal>>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};

enum LiteralType {
    BOOL_LITERAL,
    INT_LITERAL,
    REAL_LITERAL,
};

class Literal : public Entity {
public:
    LiteralType type;

    virtual std::string to_string() const {
        return "";
    }

    void accept(Visitor &) override;
};

class BoolLiteral final : public Literal {
public:
    bool value;

    std::string to_string() const override {
        return value ? "true" : "false";
    }

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    explicit BoolLiteral(const bool value)
            : value(value) {
        type = BOOL_LITERAL;
    }
};

class IntLiteral final : public Literal {
public:
    int value;

    std::string to_string() const override {
        return std::to_string(value);
    }

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return llvm::ConstantInt::get(context, llvm::APInt(32, value, true));
    }

    explicit IntLiteral(const int value)
            : value(value) {
        type = INT_LITERAL;
    }
};

class RealLiteral final : public Literal {
public:
    long double value;

    std::string to_string() const override {
        return std::to_string(value);
    }

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return llvm::ConstantFP::get(context, llvm::APFloat((double) value));
    }

    explicit RealLiteral(const long double value)
            : value(value) {
        type = REAL_LITERAL;
    }
};


class Arguments final : public Entity {
public:
    std::unique_ptr<Expressions> expressions;

    explicit Arguments(std::unique_ptr<Expressions>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};


class ClassName final : public Entity {
public:
    std::string name;
    Span span;
    std::unique_ptr<ClassName> className;

    ClassName(std::string, const Span &, std::unique_ptr<ClassName> className = nullptr);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};

class ProgramDeclaration final : public Entity {
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<ProgramArguments> arguments;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};


class Program final : public Entity {
public:
    std::unique_ptr<ProgramDeclaration> programDeclaration;
    std::unique_ptr<ClassDeclarations> classDeclarations;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;


    void accept(Visitor &) override;
};

class ClassDeclarations final : public Entity {
public:
    std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class ClassDeclaration final : public Entity {
public:
    std::unique_ptr<ClassName> className;
    std::unique_ptr<Extension> extension;
    std::unique_ptr<ClassBody> classBody;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class MemberDeclarations final : public Entity {
public:
    std::vector<std::unique_ptr<MemberDeclaration>> member_declarations;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

enum StatementType {
    ASSIGNMENT,
    WHILE_LOOP,
    IF_STATEMENT,
    RETURN_STATEMENT,
    COMPOUND_STATEMENT,
};

class Statement : public Entity {
public:
    BodyDeclaration *parent = nullptr;
    StatementType statementType;

    void accept(Visitor &) override;
};


class Extension final : public Entity {
public:
    std::unique_ptr<ClassName> className;

    explicit Extension(std::unique_ptr<ClassName>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class ClassBody final : public Entity {
public:
    std::unique_ptr<MemberDeclarations> memberDeclarations;

    explicit ClassBody(std::unique_ptr<MemberDeclarations>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class Expressions final : public Entity {
public:
    std::vector<std::unique_ptr<Expression>> expressions;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};


class Expression : public Statement {
public:
    std::string type;
    Span span;
    bool isCompound;

    std::string get_type(ScopedSymbolTable& symbolTable);

    Expression() : span(Span(0, 0, 0)) {}
};

class Primary final : public Expression {
public:

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    explicit Primary(std::unique_ptr<ClassName> &, const std::string &, const Span &);

    explicit Primary(std::unique_ptr<Literal> &, const std::string &, const Span &);

    std::unique_ptr<Literal> literal = nullptr;

    std::unique_ptr<ClassName> class_name = nullptr;

    std::string to_string() const {
        if (literal) {
            return literal->to_string();
        }
        if (class_name) {
            return class_name->name;
        }
        return "";
    }


    void accept(Visitor &) override;
};

class CompoundExpression final : public Expression {
public:
    std::string identifier;

    std::unique_ptr<Arguments> arguments;
    std::vector<std::unique_ptr<CompoundExpression>> compoundExpressions;


    explicit CompoundExpression(std::string, const Span &sp, std::unique_ptr<Arguments> args = nullptr,
                                std::vector<std::unique_ptr<CompoundExpression>> compExpr = {});


    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable, llvm::Value* prevValue,std::string prevValueType);
    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override{
       return this->codegen(context, builder, module, symbolTable, nullptr, "");
    };


    void accept(Visitor &) override;
};

class ProgramArguments final : public Entity {
public:
    std::unique_ptr<Literals> literals;

    explicit ProgramArguments(std::unique_ptr<Literals>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};

class MemberDeclaration final : public Entity {
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration;
    std::unique_ptr<MethodDeclaration> methodDeclaration;
    std::unique_ptr<ConstructorDeclaration> constructorDeclaration;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};



class ConstructorDeclaration final : public Entity {
public:
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<Body> body;
    Span span;
    std::string className;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    ConstructorDeclaration(std::unique_ptr<Parameters>, std::unique_ptr<Body>, const Span &, std::string);

    void accept(Visitor &) override;
};

class VariableDeclaration final : public Entity {
public:
    std::unique_ptr<VariableName> variable;
    std::unique_ptr<Expression> expression;
    BodyDeclaration *bodyParent = nullptr;
    MemberDeclaration *memberParent = nullptr;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    VariableDeclaration(std::unique_ptr<VariableName>, std::unique_ptr<Expression>);

    void accept(Visitor &) override;
};

class MethodDeclaration final : public Entity {
public:
    std::unique_ptr<MethodName> methodName;
    std::unique_ptr<Parameters> parameters;
    std::unique_ptr<ReturnType> returnType;
    std::unique_ptr<Body> body;

    MethodDeclaration(std::unique_ptr<MethodName> &, std::unique_ptr<Parameters> &,
                      std::unique_ptr<ReturnType> &, std::unique_ptr<Body> &);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class Parameters final : public Entity {
public:
    std::vector<std::unique_ptr<Parameter>> parameters;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};

class MethodName final : public Entity {
public:
    std::string name;
    Span span;

    explicit MethodName(std::string, const Span &);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};

class Parameter final : public Entity {
public:
    std::string name;
    Span span;
    std::unique_ptr<ClassName> className;

    Parameter(std::string, const Span &, std::unique_ptr<ClassName>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    void accept(Visitor &) override;
};

class VariableName final : public Entity {
public:
    std::string name;
    Span span;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    explicit VariableName(std::string, const Span &);


    void accept(Visitor &) override;
};

class ReturnType final : public Entity {
public:
    std::unique_ptr<ClassName> className;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override {
        return nullptr;
    }

    explicit ReturnType(std::unique_ptr<ClassName>);

    void accept(Visitor &) override;
};

class BodyDeclaration final : public Entity {
public:
    BodyDeclarations *parent = nullptr;
    std::unique_ptr<VariableDeclaration> variableDeclaration = nullptr;
    std::unique_ptr<Statement> statement = nullptr;

    explicit BodyDeclaration(std::unique_ptr<VariableDeclaration>);

    explicit BodyDeclaration(std::unique_ptr<Statement>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;


    void accept(Visitor &) override;
};


class Assignment final : public Statement {
public:
    std::unique_ptr<VariableName> variableName;
    std::unique_ptr<Expression> expression;

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    Assignment() {
        statementType = ASSIGNMENT;
    }

    void accept(Visitor &) override;
};

class WhileLoop final : public Statement {
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Body> body;

    WhileLoop(std::unique_ptr<Expression>, std::unique_ptr<Body>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;


};

class IfStatement final : public Statement {
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<IfBranch> ifBranch;
    std::unique_ptr<ElseBranch> elseBranch;

    IfStatement(std::unique_ptr<Expression>, std::unique_ptr<IfBranch>,
                std::unique_ptr<ElseBranch> else_branch = nullptr);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;


    void accept(Visitor &) override;
};

class IfBranch final : public Entity {
public:
    std::unique_ptr<Body> body;

    explicit IfBranch(std::unique_ptr<Body>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class ElseBranch final : public Entity {
public:
    std::unique_ptr<Body> body;

    explicit ElseBranch(std::unique_ptr<Body>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class ReturnStatement final : public Statement {
public:
    std::unique_ptr<Expression> expression;

    explicit ReturnStatement(std::unique_ptr<Expression>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class Body final : public Entity {
public:
    std::unique_ptr<BodyDeclarations> bodyDeclarations;

    explicit Body(std::unique_ptr<BodyDeclarations>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};

class BodyDeclarations final : public Entity {
public:
    std::vector<std::unique_ptr<BodyDeclaration>> bodyDeclarations;

    explicit BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>>);

    llvm::Value *codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module, ScopedSymbolTable& symbolTable) override;

    void accept(Visitor &) override;
};
