//
// Created by dmitriy on 12/2/24.
//

#pragma once

#include "../syntax_analyzer/AST.h"
#include "../semantics_analyzer/SymbolTable.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <string>
#include <stack>

class CodegenVisitor final : public Visitor
{
    llvm::Value* popValue();
    std::stack<llvm::Value*> valueStack;
public:
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    ScopedSymbolTable& symbolTable;

    CodegenVisitor(const std::string& moduleName, ScopedSymbolTable& symTable)
        : builder(context), module(std::make_unique<llvm::Module>(moduleName, context)),
          symbolTable(symTable)
    {
    }

    // Generate code by traversing the AST
    void generateCode(Entity& root);

    // Retrieve the generated value for the root node
    llvm::Value* getGeneratedValue() const;

    void visitProgram(Program&) override;

    void visitProgramDeclaration(ProgramDeclaration&) override;

    void visitClassName(ClassName&) override;

    void visitProgramArguments(ProgramArguments&) override;

    void visitLiterals(Literals&) override;

    void visitLiteral(Literal&) override;

    void visitArguments(Arguments&) override;

    void visitExpressions(Expressions&) override;

    void visitExpression(Expression&) override;

    void visitPrimary(Primary&) override;

    void visitCompoundExpression(CompoundExpression&) override;

    void visitClassDeclarations(ClassDeclarations&) override;

    void visitClassDeclaration(ClassDeclaration&) override;

    void visitClassBody(ClassBody&) override;

    void visitExtension(Extension&) override;

    void visitBody(Body&) override;

    void visitBodyDeclarations(BodyDeclarations&) override;

    void visitBodyDeclaration(BodyDeclaration&) override;

    void visitStatement(Statement&) override;

    void visitIfStatement(IfStatement&) override;

    void visitIfBranch(IfBranch&) override;

    void visitElseBranch(ElseBranch&) override;

    void visitWhileLoop(WhileLoop&) override;

    void visitAssignment(Assignment&) override;

    void visitMemberDeclarations(MemberDeclarations&) override;

    void visitMemberDeclaration(MemberDeclaration&) override;

    void visitConstructorDeclaration(ConstructorDeclaration&) override;

    void visitReturnStatement(ReturnStatement&) override;

    void visitVariableDeclaration(VariableDeclaration&) override;

    void visitMethodDeclaration(MethodDeclaration&) override;

    void visitMethodName(MethodName&) override;

    void visitParameters(Parameters&) override;

    void visitParameter(Parameter&) override;

    void visitReturnType(ReturnType&) override;

    void visitVariableName(VariableName&) override;

    // Implement other visit methods as needed
};
