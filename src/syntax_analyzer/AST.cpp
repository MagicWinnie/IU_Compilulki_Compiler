//
// Created by Nail Minnemullin on 08.10.2024.
//

#include <iostream>
#include <utility>
#include "AST.h"
#include "../code_generator/LLVMSymbolTable.h"

Entity::~Entity() = default;

LLVMScopedSymbolTable symbolTable;

void Entity::printIndent(const int indent) {
    for (int i = 0; i < indent; ++i) std::cout << "  ";
}


Literals::Literals(std::vector<std::unique_ptr<Literal>> literals) : literals(std::move(literals)) {
}

void Literals::accept(Visitor &visitor) {
    visitor.visitLiterals(*this);
}


void Literal::accept(Visitor &visitor) {
    visitor.visitLiteral(*this);
}

Arguments::Arguments(std::unique_ptr<Expressions> expressions) : expressions(std::move(expressions)) {
}

void Arguments::accept(Visitor &visitor) {
    visitor.visitArguments(*this);
}

ClassName::ClassName(std::string name, const Span &span, std::unique_ptr<ClassName> className) :
        name(std::move(name)), span(span), className(std::move(className)) {
}

void ClassName::accept(Visitor &visitor) {
    visitor.visitClassName(*this);
}

void ProgramDeclaration::accept(Visitor &visitor) {
    visitor.visitProgramDeclaration(*this);
}

llvm::Value *ProgramDeclaration::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {

// TODO to other place REFACTOR!!


    // Step 1: Create or get the 'main' function.
    llvm::FunctionType *mainFuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context),
                                                               false);  // main returns an int32 (exit code)
    llvm::Function *mainFunc = llvm::Function::Create(mainFuncType, llvm::Function::ExternalLinkage, "main", module);

    // Step 2: Create the entry block for the main function
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);  // Set the insertion point to the entry block of the main function

    // Step 3: Get the struct type for the class (e.g., 'Main') using className
    llvm::StructType *mainType = llvm::StructType::getTypeByName(context, className->name);

    // Step 4: Allocate memory for the 'Main' object
    llvm::AllocaInst *mainAlloc = builder.CreateAlloca(mainType, nullptr, "main_object");

    // Handle arguments
    std::vector<llvm::Value *> args;
    args.push_back(mainAlloc);  // Add the 'Main' object as the first argument
    if (arguments) {
        for (auto &arg: arguments->literals->literals) {
            llvm::Value *argValue = arg->codegen(context, builder, module);
            if (!argValue) {
                llvm::errs() << "Error: Failed to generate code for argument.\n";
                return nullptr;
            }
            args.push_back(argValue);  // Add argument value to the call list
        }
    }

    // Step 5: Retrieve the constructor function for Main (e.g., '@Main_Create_Default')
    std::string constructor_name = className->name + "_Create_Default";
    llvm::Function *constructorFunc = module.getFunction(constructor_name);
    if (!constructorFunc) {
        llvm::errs() << "Error: Constructor function not found for " << constructor_name << "\n";
        return nullptr;
    }

    // Step 6: Call the constructor function to initialize the 'Main' object
    builder.CreateCall(constructorFunc, args);


    // Step 7: Optionally, return the object or return an exit code from main
    builder.CreateRet(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0));  // Return 0 for a successful execution

    return mainAlloc;
}

void Program::accept(Visitor &visitor) {
    visitor.visitProgram(*this);
}

llvm::Value *Program::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    symbolTable.enterScope();
    symbolTable.addIdentifier("Integer", CLASS);
    symbolTable.addIdentifier("Boolean", CLASS);
    symbolTable.addIdentifier("Real", CLASS);
    if (classDeclarations) {
        classDeclarations->codegen(context, builder, module);
    }
    if (programDeclaration) {
        programDeclaration->codegen(context, builder, module);
    }
    symbolTable.leaveScope();
    return nullptr;
}

void ClassDeclarations::accept(Visitor &visitor) {
    visitor.visitClassDeclarations(*this);
}

llvm::Value *ClassDeclarations::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    for (const auto &classDeclaration: classDeclarations) {
        classDeclaration->codegen(context, builder, module);
    }
    return nullptr;
}

void ClassDeclaration::accept(Visitor &visitor) {
    visitor.visitClassDeclaration(*this);
}

llvm::Value *ClassDeclaration::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    symbolTable.enterScope();

    // TODO fix if class name has elements
    llvm::StructType *classType = llvm::StructType::create(context, {}, className->name);
    if (classBody) classBody->codegen(context, builder, module);
    symbolTable.addIdentifier(className->name, CLASS);
    symbolTable.leaveScope();
    return nullptr;


}

void MemberDeclarations::accept(Visitor &visitor) {
    visitor.visitMemberDeclarations(*this);
}

llvm::Value *MemberDeclarations::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    for (const auto &memberDeclaration: member_declarations) {
        memberDeclaration->codegen(context, builder, module);
    }
    return nullptr;
}

Extension::Extension(std::unique_ptr<ClassName> class_name) : className(std::move(class_name)) {
}

void Extension::accept(Visitor &visitor) {
    visitor.visitExtension(*this);
}

llvm::Value *Extension::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (className) {
        className->codegen(context, builder, module);
    }
    return nullptr;
}

ClassBody::ClassBody(std::unique_ptr<MemberDeclarations> member_declarations) : memberDeclarations(
        std::move(member_declarations)) {
}

void ClassBody::accept(Visitor &visitor) {
    visitor.visitClassBody(*this);
}

llvm::Value *ClassBody::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (memberDeclarations) {
        memberDeclarations->codegen(context, builder, module);
    }
    return nullptr;
}

void Expressions::accept(Visitor &visitor) {
    visitor.visitExpressions(*this);
}


void Primary::accept(Visitor &visitor) {
    visitor.visitPrimary(*this);
}

llvm::Value *Primary::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (literal) {
        return literal->codegen(context, builder, module);
    }
    //TODO class name
    return nullptr;

}

Primary::Primary(std::unique_ptr<ClassName> &class_name, const std::string &type, const Span &span) : class_name(
        std::move(class_name)) {
    isCompound = false;
    this->span = span;
    this->type = type;
}

Primary::Primary(std::unique_ptr<Literal> &literal, const std::string &type, const Span &) {
    this->literal = std::move(literal);
    isCompound = false;
    this->span = span;
    this->type = type;

}

CompoundExpression::CompoundExpression(std::string id, const Span &span, std::unique_ptr<Arguments> args,
                                       std::vector<std::unique_ptr<CompoundExpression>> compExpr) :
        identifier(std::move(id)), arguments(std::move(args)), compoundExpressions(std::move(compExpr)) {
    statementType = COMPOUND_STATEMENT;
    isCompound = true;
    this->span = span;
}

void CompoundExpression::accept(Visitor &visitor) {
    visitor.visitCompoundExpression(*this);
}

llvm::Value *CompoundExpression::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    llvm::Value *value = nullptr;
    if (compoundExpressions.empty()) {

        IdentifierType type = symbolTable.getIdentifierType(identifier);
        // Get local value

        if (type == VARIABLE) {
            llvm::Value *ptr = symbolTable.getLocalVariable(identifier);
            return ptr;
        }
        if (type == CLASS) {

            llvm::StructType *classType = llvm::StructType::getTypeByName(context, identifier);
            llvm::AllocaInst *classAlloc = builder.CreateAlloca(classType, nullptr, "class_object");
            std::string constructorName = identifier + "_Create_Default";
            llvm::Function *constructorFunc = module.getFunction(constructorName);
            if (!constructorFunc) {
                llvm::errs() << "Error: " << constructorName << " not found.\n";
                return nullptr;
            }
            std::vector<llvm::Value *> constructorArgs;
            constructorArgs.push_back(classAlloc);
            if (arguments) {
                for (auto &arg: arguments->expressions->expressions) {
                    llvm::Value *argValue = arg->codegen(context, builder, module);
                    if (!argValue) {
                        llvm::errs() << "Error: Failed to generate code for constructor argument.\n";
                        return nullptr;
                    }
                    constructorArgs.push_back(argValue); // Add argument value to the call list
                }
            }
            builder.CreateCall(constructorFunc, constructorArgs);
            return classAlloc;

        }
        if(type==FUNCTION){
            llvm::Function *function = module.getFunction(identifier);
            if (!function) {
                llvm::errs() << "Error: " << identifier << " not found.\n";
                return nullptr;
            }
            if (function->getReturnType()->isVoidTy()) {
                // TODO add other arguments
                value = builder.CreateCall(function, {});
            } else {
                // TODO add other arguments
                value = builder.CreateCall(function, {}, "call_" + identifier);
            }
        }

    } else {
        IdentifierType type = symbolTable.getIdentifierType(identifier);

        if (type == VARIABLE) {
            llvm::Value *currentValue = symbolTable.getLocalVariable(identifier);
            auto varType = symbolTable.getLocalVariableType(identifier);
            auto funcName = varType + "_" + compoundExpressions[0]->identifier;
            llvm::Function *function = module.getFunction(funcName);

            if (!function) {
                return nullptr;
            }
            if (function->getReturnType()->isVoidTy()) {
                // TODO add other arguments
                value = builder.CreateCall(function, {currentValue});
            } else {
                // TODO add other arguments
                value = builder.CreateCall(function, {currentValue}, "call_" + funcName);
            }
        } else if (type == CLASS) {
            123;
        }


    }
    return nullptr;
}

ProgramArguments::ProgramArguments(std::unique_ptr<Literals> literals) : literals(std::move(literals)) {
}

void ProgramArguments::accept(Visitor &visitor) {
    visitor.visitProgramArguments(*this);
}

void MemberDeclaration::accept(Visitor &visitor) {
    visitor.visitMemberDeclaration(*this);
}

llvm::Value *MemberDeclaration::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (variableDeclaration) {
        variableDeclaration->codegen(context, builder, module);
    }
    if (methodDeclaration) {
        methodDeclaration->codegen(context, builder, module);
    }
    if (constructorDeclaration) {
        constructorDeclaration->codegen(context, builder, module);
    }
    return nullptr;
}

ConstructorDeclaration::ConstructorDeclaration(std::unique_ptr<Parameters> parameters, std::unique_ptr<Body> body,
                                               const Span &span, std::string className) :
        className(std::move(className)),
        parameters(std::move(parameters)),
        body(std::move(body)),
        span(span) {
}

void ConstructorDeclaration::accept(Visitor &visitor) {
    visitor.visitConstructorDeclaration(*this);
}

llvm::Value *
ConstructorDeclaration::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    symbolTable.enterScope();
    llvm::StructType *classType = llvm::StructType::getTypeByName(context, className);
    llvm::FunctionType *constructorType = llvm::FunctionType::get(llvm::Type::getVoidTy(context),
                                                                  {classType->getPointerTo()}, false);

    // Generate the constructor function (name it based on the class it's constructing).

    // TODO class may have two or more constructors
    // TODO class may have arguments
    llvm::Function *constructor = llvm::Function::Create(constructorType, llvm::Function::ExternalLinkage,
                                                         className + "_Create_Default", module);

    // Create the entry basic block for the constructor.
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", constructor);
    builder.SetInsertPoint(entry);

    // Get the 'this' pointer (the first argument of the constructor).
    llvm::Value *thisPtr = constructor->getArg(0);

    if (body) {
        body->codegen(context, builder, module);
    }

    // Finally, return void from the constructor function.
    builder.CreateRetVoid();


    symbolTable.leaveScope();

    return nullptr;
}

VariableDeclaration::VariableDeclaration(std::unique_ptr<VariableName> variable,
                                         std::unique_ptr<Expression> expression) : variable(std::move(variable)),
                                                                                   expression(std::move(expression)) {
}

void VariableDeclaration::accept(Visitor &visitor) {
    visitor.visitVariableDeclaration(*this);
}

llvm::Value *
VariableDeclaration::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (expression->isCompound) {
        auto val = expression->codegen(context, builder, module);
        CompoundExpression *expr = dynamic_cast<CompoundExpression *>(expression.get());
        symbolTable.addLocalVariable(variable->name, val, expr->identifier);
        symbolTable.addIdentifier(variable->name, VARIABLE);
        return val;
    } else {
        Primary *expr = dynamic_cast<Primary *>(expression.get());
        llvm::StructType *classType = llvm::StructType::getTypeByName(context, expr->type);
        llvm::AllocaInst *classAlloc = builder.CreateAlloca(classType, nullptr, variable->name);
        std::string constructorName = expr->type + "_Create_Default";
        llvm::Function *constructorFunc = module.getFunction(constructorName);
        if (!constructorFunc) {
            llvm::errs() << "Error: " << constructorName << " not found.\n";
            return nullptr;
        }
        std::vector<llvm::Value *> constructorArgs;
        constructorArgs.push_back(classAlloc);
        if (expr->literal) {
            if (expr->literal->type == BOOL_LITERAL) {
                BoolLiteral *boolLiteral = dynamic_cast<BoolLiteral *>(expr->literal.get());
                llvm::Value *boolValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), boolLiteral->value);
                constructorArgs.push_back(boolValue);
            } else if (expr->literal->type == INT_LITERAL) {
                IntLiteral *intLiteral = dynamic_cast<IntLiteral *>(expr->literal.get());
                llvm::Value *intValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), intLiteral->value);
                constructorArgs.push_back(intValue);
            } else if (expr->literal->type == REAL_LITERAL) {
                RealLiteral *realLiteral = dynamic_cast<RealLiteral *>(expr->literal.get());
                llvm::Value *realValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context),
                                                               (double) realLiteral->value);
                constructorArgs.push_back(realValue);
            }
        }

        builder.CreateCall(constructorFunc, constructorArgs);
        symbolTable.addLocalVariable(variable->name, classAlloc, expr->type);
        symbolTable.addIdentifier(variable->name, VARIABLE);
        return classAlloc;
    }

    return nullptr;
}

MethodDeclaration::MethodDeclaration(std::unique_ptr<MethodName> &method_name, std::unique_ptr<Parameters> &parameters,
                                     std::unique_ptr<ReturnType> &return_type,
                                     std::unique_ptr<Body> &body) : methodName(std::move(method_name)),
                                                                    parameters(std::move(parameters)),
                                                                    returnType(std::move(return_type)),
                                                                    body(std::move(body)) {
}

void MethodDeclaration::accept(Visitor &visitor) {
    visitor.visitMethodDeclaration(*this);
}

llvm::Value *MethodDeclaration::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    // Step 1: Get the function return type
    symbolTable.enterScope();


    llvm::Type *llvmReturnType = nullptr;
    if (returnType == nullptr) {
        llvmReturnType = llvm::Type::getVoidTy(context);
    } else {
        llvmReturnType = llvm::StructType::getTypeByName(context, returnType->className->name);
    }

    if (!llvmReturnType) {
        llvm::errs() << "Error: Failed to generate return type for method " << methodName->name << "\n";
        return nullptr;
    }

    // Step 2: Generate function parameters
    std::vector<llvm::Type *> paramTypes;
    std::vector<std::string> paramNames;
    if (parameters) {
        for (const auto &param: parameters->parameters) {
            llvm::Type *paramType = llvm::StructType::getTypeByName(context, param->className->name);
            if (!paramType) {
                llvm::errs() << "Error: Failed to generate code for parameter " << param->name << "\n";
                return nullptr;
            }
            paramTypes.push_back(paramType);
            paramNames.push_back(param->name);
        }
    }
//
    llvm::FunctionType *functionType = llvm::FunctionType::get(llvmReturnType, paramTypes, false);
    llvm::Function *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, methodName->name,
                                                      &module);
//
    // Step 3: Name the function arguments
    size_t idx = 0;
    for (auto &arg: function->args()) {
        arg.setName(paramNames[idx++]);
    }

//    // Step 4: Create a new basic block to start inserting into
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(entryBlock);
//
//    // Step 5: Add function parameters to the symbol table
    idx = 0;
    for (auto &arg: function->args()) {
        llvm::AllocaInst *alloca = builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
        builder.CreateStore(&arg, alloca);
        std::string argName = arg.getName().str();
        symbolTable.addLocalVariable(argName, alloca, paramTypes[idx++]->getStructName().str());
    }

//    // Step 6: Generate code for the function body
    llvm::Value *bodyVal = body->codegen(context, builder, module);
//
//    // Step 7: Return the generated value (for non-void functions)
    if (llvmReturnType->isVoidTy()) {
        builder.CreateRetVoid();
    } else {
        if (bodyVal->getType()->isPointerTy()) {
            bodyVal = builder.CreateLoad(llvmReturnType, bodyVal, "returnVal");
        }
        builder.CreateRet(bodyVal);
    }
    symbolTable.addIdentifier(methodName->name, FUNCTION);
    llvm::verifyFunction(*function);
    symbolTable.leaveScope();
    return function;
}


void Parameters::accept(Visitor &visitor) {
    visitor.visitParameters(*this);
}

MethodName::MethodName(std::string name, const Span &span) : name(std::move(name)), span(span) {
}

void MethodName::accept(Visitor &visitor) {
    visitor.visitMethodName(*this);
}

Parameter::Parameter(std::string name, const Span &span, std::unique_ptr<ClassName> class_name) :
        name(std::move(name)), span(span), className(std::move(class_name)) {
}

void Parameter::accept(Visitor &visitor) {
    visitor.visitParameter(*this);
}

VariableName::VariableName(std::string name, const Span &span) : name(std::move(name)), span(span) {
}

void VariableName::accept(Visitor &visitor) {
    visitor.visitVariableName(*this);
}

ReturnType::ReturnType(std::unique_ptr<ClassName> class_name) : className(std::move(class_name)) {
}

void ReturnType::accept(Visitor &visitor) {
    visitor.visitReturnType(*this);
}

BodyDeclaration::BodyDeclaration(std::unique_ptr<VariableDeclaration> variable_declaration) : variableDeclaration(
        std::move(variable_declaration)) {
}

BodyDeclaration::BodyDeclaration(std::unique_ptr<Statement> statement) : statement(std::move(statement)) {
}

void BodyDeclaration::accept(Visitor &visitor) {
    visitor.visitBodyDeclaration(*this);
}

llvm::Value *BodyDeclaration::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (variableDeclaration) {
        return variableDeclaration->codegen(context, builder, module);
    }
    if (statement) {
        return statement->codegen(context, builder, module);
    }
    return nullptr;
}

void Statement::accept(Visitor &visitor) {
    visitor.visitStatement(*this);
}


void Assignment::accept(Visitor &visitor) {
    visitor.visitAssignment(*this);
}

llvm::Value *Assignment::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (expression->isCompound) {
        llvm::Value *value = expression->codegen(context, builder, module);
        if (!value) {
            llvm::errs() << "Error: Failed to generate code for compound assignment expression.\n";
            return nullptr;
        }

        // Get the destination pointer for the assignment
        llvm::Value *ptr = symbolTable.getLocalVariable(variableName->name);
        if (!ptr) {
            llvm::errs() << "Error: Undefined local variable " << variableName->name << "\n";
            return nullptr;
        }

        // Use memcpy to copy the value
        llvm::Type *valueType = value->getType();
        llvm::DataLayout dataLayout(&module); // Get the data layout from the module
        uint64_t size = dataLayout.getTypeAllocSize(valueType); // Get the size of the type

        // Create constants for memcpy parameters
        llvm::Value *sizeValue = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), size);
        llvm::Value *isVolatile = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0);

        // Call llvm.memcpy intrinsic
        // TODO maybe memmove
        llvm::Function *memcpyFunc = llvm::Intrinsic::getDeclaration(&module, llvm::Intrinsic::memcpy,
                                                                     {value->getType(), ptr->getType(),
                                                                      sizeValue->getType()}
        );
        builder.CreateCall(memcpyFunc, {ptr, value, sizeValue, isVolatile});

        return value;
    } else {
        llvm::Value *value = expression->codegen(context, builder, module);
        if (!value) {
            llvm::errs() << "Error: Failed to generate code for assignment expression.\n";
            return nullptr;
        }
        llvm::Value *ptr = symbolTable.getLocalVariable(variableName->name);
        if (!ptr) {
            llvm::errs() << "Error: Undefined local variable " << variableName->name << "\n";
            return nullptr;
        }
        builder.CreateStore(value, ptr);
        return value;
    }

}

llvm::Value *IfStatement::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    llvm::Value *pointerToBoolean = expression->codegen(context, builder, module);
    llvm::Value *valuePtr = builder.CreateStructGEP(

            llvm::StructType::getTypeByName(context, "Boolean"),
            pointerToBoolean,
            0,
            "boolFieldPtr"
    );
    llvm::Value *CondV = builder.CreateLoad(
            llvm::Type::getInt1Ty(context), // The type of the value.
            valuePtr,
            "loadBoolValue"
    );

    // Convert the condition to a boolean value (assume nonzero is true)
    CondV = builder.CreateICmpNE(CondV, llvm::ConstantInt::get(CondV->getType(), 0), "ifcond");

    // Create basic blocks for 'then', 'else', and 'merge'
    llvm::Function *TheFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(context, "btrue", TheFunction);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(context, "bfalse", TheFunction);  // Add TheFunction here
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(context, "end", TheFunction);  // Add TheFunction here

    // Create the conditional branch
    builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // Generate code for the 'then' block
    builder.SetInsertPoint(ThenBB);
    llvm::Value *ThenV = this->ifBranch->codegen(context, builder, module);

    builder.CreateBr(MergeBB);

    ThenBB = builder.GetInsertBlock();

    builder.SetInsertPoint(ElseBB);
    llvm::Value *ElseV = nullptr;
    if (this->elseBranch) {
        ElseV = this->elseBranch->codegen(context, builder, module);
    }
    builder.CreateBr(MergeBB);

    builder.SetInsertPoint(MergeBB);
    return nullptr;

}

WhileLoop::WhileLoop(std::unique_ptr<Expression> expression, std::unique_ptr<Body> body) :
        expression(std::move(expression)),
        body(std::move(body)) {
    statementType = WHILE_LOOP;
}

void WhileLoop::accept(Visitor &visitor) {
    visitor.visitWhileLoop(*this);
}

llvm::Value *WhileLoop::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    llvm::Function *TheFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(context, "loop", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(context, "afterloop", TheFunction);
    llvm::Value *pointerToBoolean = expression->codegen(context, builder, module);
    llvm::Value *valuePtr = builder.CreateStructGEP(

            llvm::StructType::getTypeByName(context, "Boolean"),
            pointerToBoolean,
            0,
            "boolFieldPtr"
    );
    llvm::Value *CondV = builder.CreateLoad(
            llvm::Type::getInt1Ty(context), // The type of the value.
            valuePtr,
            "loadBoolValue"
    );
    if (!CondV)
        return nullptr;
    CondV = builder.CreateICmpNE(CondV, llvm::ConstantInt::get(CondV->getType(), 0), "whilecond");
    builder.CreateCondBr(CondV, LoopBB, AfterBB);
    builder.SetInsertPoint(LoopBB);
    if (body) {
        body->codegen(context, builder, module);
    }

    pointerToBoolean = expression->codegen(context, builder, module);
    valuePtr = builder.CreateStructGEP(
            llvm::StructType::getTypeByName(context,
                                            "Boolean"), // This retrieves the type definition of the 'Boolean' class.
            pointerToBoolean,                              // Pointer to the object instance.
            0,                                      // Field index: assuming value is at index 0.
            "boolFieldPtr"
    );
    CondV = builder.CreateLoad(
            llvm::Type::getInt1Ty(context), // The type of the value.
            valuePtr,
            "loadBoolValue"
    );
    if (!CondV)
        return nullptr;
    CondV = builder.CreateICmpNE(CondV, llvm::ConstantInt::get(CondV->getType(), 0), "whilecond");
    builder.CreateCondBr(CondV, LoopBB, AfterBB);
    builder.SetInsertPoint(AfterBB);
    return nullptr;
}

IfStatement::IfStatement(std::unique_ptr<Expression> expression, std::unique_ptr<IfBranch> if_branch,
                         std::unique_ptr<ElseBranch> else_branch) : expression(std::move(expression)),
                                                                    ifBranch(std::move(if_branch)),
                                                                    elseBranch(std::move(else_branch)) {
    statementType = IF_STATEMENT;
}

void IfStatement::accept(Visitor &visitor) {
    visitor.visitIfStatement(*this);
}

IfBranch::IfBranch(std::unique_ptr<Body> body) : body(std::move(body)) {
}

void IfBranch::accept(Visitor &visitor) {
    visitor.visitIfBranch(*this);
}

llvm::Value *IfBranch::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (body) {
        return body->codegen(context, builder, module);
    }
    return nullptr;
}

ElseBranch::ElseBranch(std::unique_ptr<Body> body) : body(std::move(body)) {
}

void ElseBranch::accept(Visitor &visitor) {
    visitor.visitElseBranch(*this);
}

llvm::Value *ElseBranch::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (body) {
        body->codegen(context, builder, module);
    }
    return nullptr;
}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {
    statementType = RETURN_STATEMENT;
}

void ReturnStatement::accept(Visitor &visitor) {
    visitor.visitReturnStatement(*this);
}

llvm::Value *ReturnStatement::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    if (expression) {
        return expression->codegen(context, builder, module);
    }
    return nullptr;
}

Body::Body(std::unique_ptr<BodyDeclarations> body_declarations) : bodyDeclarations(std::move(body_declarations)) {
}

void Body::accept(Visitor &visitor) {
    visitor.visitBody(*this);
}

llvm::Value *Body::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {

    if (bodyDeclarations) {
        return bodyDeclarations->codegen(context, builder, module);
    }
    return nullptr;
}

BodyDeclarations::BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>> body_declarations) : bodyDeclarations(
        std::move(body_declarations)) {
}

void BodyDeclarations::accept(Visitor &visitor) {
    visitor.visitBodyDeclarations(*this);
}

llvm::Value *BodyDeclarations::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    llvm::Value *value = nullptr;
    for (const auto &bodyDeclaration: bodyDeclarations) {
        value = bodyDeclaration->codegen(context, builder, module);
    }

    return value;
}


llvm::Value *BoolLiteral::codegen(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), value);

}

std::string Expression::get_type() {
    if (isCompound) {
        // TODO fix compound expression type
        return "WTF??";
    } else {
        return type;
    }
}
