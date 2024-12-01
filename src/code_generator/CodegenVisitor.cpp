//
// Created by dmitriy on 12/2/24.
//

#include "CodegenVisitor.h"
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>

void CodegenVisitor::generateCode(Entity& root)
{
    root.accept(*this);

    // After traversal, the top of the stack should contain the root's value
    if (!valueStack.empty())
    {
        llvm::Value* rootValue = valueStack.top();
        // You can use rootValue as needed, e.g., return it or store it
        // For now, we'll just leave it on the stack
    }

    // Verify the module
    if (verifyModule(*module, &llvm::errs()))
    {
        llvm::errs() << "Error: Module verification failed\n";
    }

    // Optionally, output the LLVM IR to a file
    module->print(llvm::outs(), nullptr);
}

llvm::Value* CodegenVisitor::getGeneratedValue() const
{
    if (valueStack.empty())
    {
        return nullptr;
    }
    return valueStack.top();
}

llvm::Value* CodegenVisitor::popValue()
{
    if (valueStack.empty())
    {
        llvm::errs() << "Error: Attempted to pop from an empty value stack.\n";
        return nullptr;
    }
    llvm::Value* val = valueStack.top();
    valueStack.pop();
    return val;
}

void CodegenVisitor::visitProgram(Program& program)
{
    if (program.classDeclarations)
        program.classDeclarations->accept(*this);
    if (program.programDeclaration)
        program.programDeclaration->accept(*this);
}

void CodegenVisitor::visitProgramDeclaration(ProgramDeclaration& programDecl)
{
    // Step 1: Create or get the 'main' function.
    llvm::FunctionType* mainFuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    // main returns an int32 (exit code)
    llvm::Function* mainFunc = llvm::Function::Create(mainFuncType, llvm::Function::ExternalLinkage, "main",
                                                      module.get());

    // Step 2: Create the entry block for the main function
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry); // Set the insertion point to the entry block of the main function

    // Step 3: Get the struct type for the class (e.g., 'Main') using className
    llvm::StructType* mainType = llvm::StructType::getTypeByName(context, programDecl.className->name);
    if (!mainType)
    {
        llvm::errs() << "Error: Struct type '" << programDecl.className->name << "' not found.\n";
        return;
    }

    // Step 4: Allocate memory for the 'Main' object
    llvm::AllocaInst* mainAlloc = builder.CreateAlloca(mainType, nullptr, "main_object");

    // Handle arguments
    std::vector<llvm::Value*> args;
    std::vector<std::string> argTypes;
    args.push_back(mainAlloc); // Add the 'Main' object as the first argument
    if (programDecl.arguments)
    {
        for (const auto& arg : programDecl.arguments->literals->literals)
        {
            // Visit the argument to generate its LLVM value
            arg->accept(*this);
            llvm::Value* argValue = popValue();
            if (!argValue)
            {
                llvm::errs() << "Error: Failed to generate code for argument.\n";
                return;
            }
            args.push_back(argValue); // Add argument value to the call list
            if (arg->type == INT_LITERAL)
            {
                argTypes.emplace_back("Integer");
            }
            else if (arg->type == REAL_LITERAL)
            {
                argTypes.emplace_back("Real");
            }
            else if (arg->type == BOOL_LITERAL)
            {
                argTypes.emplace_back("Boolean");
            }
            // Handle other literal types as necessary
        }
    }

    // Step 5: Retrieve the constructor function for Main (e.g., '@Main_Create_Integer_Boolean')
    std::string constructor_name = programDecl.className->name + "_Constructor";
    for (const auto& argType : argTypes)
    {
        constructor_name += "_" + argType;
    }

    llvm::Function* constructorFunc = module->getFunction(constructor_name);
    if (!constructorFunc)
    {
        llvm::errs() << "Error: Constructor function not found for " << constructor_name << "\n";
        return;
    }

    // Step 6: Call the constructor function to initialize the 'Main' object
    builder.CreateCall(constructorFunc, args, "call_ctor");

    // Step 7: Optionally, return the object or return an exit code from main
    builder.CreateRet(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0)); // Return 0 for a successful execution

    // Push the return value (exit code) onto the stack if needed
    llvm::Value* exitCode = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    valueStack.push(exitCode);
}

void CodegenVisitor::visitClassName(ClassName& className)
{
}

void CodegenVisitor::visitProgramArguments(ProgramArguments& node)
{
    if (node.literals) node.literals->accept(*this);
}

void CodegenVisitor::visitLiterals(Literals& node)
{
    for (const auto& literal : node.literals)
    {
        literal->accept(*this);
    }
}

void CodegenVisitor::visitLiteral(Literal& node)
{
    if (node.type == BOOL_LITERAL)
    {
        const auto nodePtr = dynamic_cast<BoolLiteral&>(node);
        valueStack.push(llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), nodePtr.value));
    }
    else if (node.type == INT_LITERAL)
    {
        const auto nodePtr = dynamic_cast<IntLiteral&>(node);
        valueStack.push(llvm::ConstantInt::get(context, llvm::APInt(32, nodePtr.value, true)));
    }
    else if (node.type == REAL_LITERAL)
    {
        const auto nodePtr = dynamic_cast<RealLiteral&>(node);
        valueStack.push(llvm::ConstantFP::get(context, llvm::APFloat(static_cast<double>(nodePtr.value))));
    }
}

void CodegenVisitor::visitArguments(Arguments& node)
{
    if (node.expressions) node.expressions->accept(*this);
}

void CodegenVisitor::visitExpressions(Expressions& node)
{
    for (const auto& expression : node.expressions)
    {
        expression->accept(*this);
    }
}

void CodegenVisitor::visitExpression(Expression& node)
{
    node.accept(*this);
}
