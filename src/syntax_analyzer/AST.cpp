//
// Created by Nail Minnemullin on 08.10.2024.
//

#include <iostream>
#include <utility>
#include "AST.h"

Visitor::Visitor() = default;
Visitor::~Visitor() = default;

std::string Literal::to_string() const
{
    return "";
}

std::string BoolLiteral::to_string() const
{
    return value ? "true" : "false";
}

BoolLiteral::BoolLiteral(const bool value): value(value)
{
    type = BOOL_LITERAL;
}

std::string IntLiteral::to_string() const
{
    return std::to_string(value);
}

IntLiteral::IntLiteral(const int value): value(value)
{
    type = INT_LITERAL;
}

std::string RealLiteral::to_string() const
{
    return std::to_string(value);
}

RealLiteral::RealLiteral(const long double value): value(value)
{
    type = REAL_LITERAL;
}

Expression::Expression(): span(Span(0, 0, 0)), isCompound(false)
{
}

std::string Primary::to_string() const
{
    if (literal)
    {
        return literal->to_string();
    }
    if (class_name)
    {
        return class_name->name;
    }
    return "";
}

llvm::Value* CompoundExpression::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                         ScopedSymbolTable& symbolTable)
{
    return this->codegen(context, builder, module, symbolTable, nullptr, "");
}

void VariableDeclaration::markForDeletion()
{
    needsToBeDeleted = true;
}

llvm::Value* Parameters::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                 ScopedSymbolTable& symbolTable)
{
    return nullptr;
}

llvm::Value* MethodName::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                 ScopedSymbolTable& symbolTable)
{
    return nullptr;
}

llvm::Value* Parameter::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                ScopedSymbolTable& symbolTable)
{
    return nullptr;
}

llvm::Value* VariableName::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                   ScopedSymbolTable& symbolTable)
{
    return nullptr;
}

llvm::Value* ReturnType::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                 ScopedSymbolTable& symbolTable)
{
    return nullptr;
}

Assignment::Assignment()
{
    statementType = ASSIGNMENT;
}

Entity::~Entity() = default;


void Entity::printIndent(const int indent)
{
    for (int i = 0; i < indent; ++i) std::cout << "  ";
}


Literals::Literals(std::vector<std::unique_ptr<Literal>> literals) : literals(std::move(literals))
{
}

void Literals::accept(Visitor& visitor)
{
    visitor.visitLiterals(*this);
}


void Literal::accept(Visitor& visitor)
{
    visitor.visitLiteral(*this);
}

Arguments::Arguments(std::unique_ptr<Expressions> expressions) : expressions(std::move(expressions))
{
}

void Arguments::accept(Visitor& visitor)
{
    visitor.visitArguments(*this);
}

ClassName::ClassName(std::string name, const Span& span, std::unique_ptr<ClassName> className) : name(std::move(name)),
    span(span), className(
        std::move(className))
{
}

void ClassName::accept(Visitor& visitor)
{
    visitor.visitClassName(*this);
}

void ProgramDeclaration::accept(Visitor& visitor)
{
    visitor.visitProgramDeclaration(*this);
}

void Program::accept(Visitor& visitor)
{
    visitor.visitProgram(*this);
}

void ClassDeclarations::accept(Visitor& visitor)
{
    visitor.visitClassDeclarations(*this);
}

llvm::Value* ClassDeclarations::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                        ScopedSymbolTable& symbolTable)
{
    for (const auto& classDeclaration : classDeclarations)
    {
        classDeclaration->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

void ClassDeclaration::accept(Visitor& visitor)
{
    visitor.visitClassDeclaration(*this);
}

llvm::Value* ClassDeclaration::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                       ScopedSymbolTable& symbolTable)
{
    auto elements = symbolTable.lookupClass(className->name, className->span)->getFields();
    std::vector<llvm::Type*> elementsType;
    for (const auto& element : elements)
    {
        // All my types are structs
        elementsType.push_back(llvm::StructType::getTypeByName(context, element.type));
    }

    llvm::StructType* classType = llvm::StructType::create(context, elementsType, className->name);
    symbolTable.currClassName = className->name;

    std::vector<llvm::Type*> constructorArgs;
    constructorArgs.push_back(llvm::PointerType::get(classType, 0));

    // Create init block
    std::string initName = className->name + "_Init";
    llvm::FunctionType* initType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), constructorArgs, false);
    llvm::Function* initFunc = llvm::Function::Create(initType, llvm::Function::ExternalLinkage, initName,
                                                      module);
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", initFunc);
    builder.SetInsertPoint(entry);
    if (extension)
    {
        // Crete call to the base class init using this pointer
        std::string baseClassName = extension->className->name;
        std::string baseInitName = baseClassName + "_Init";
        llvm::Function* baseInitFunc = module.getFunction(baseInitName);
        if (!baseInitFunc)
        {
            llvm::errs() << "Error: Base init function not found for " << baseInitName << "\n";
            return nullptr;
        }
        std::vector<llvm::Value*> baseInitArgs;
        baseInitArgs.push_back(initFunc->getArg(0));
        builder.CreateCall(baseInitFunc, baseInitArgs);
    }
    builder.CreateRetVoid();

    if (classBody) classBody->codegen(context, builder, module, symbolTable);

    // Find the constructor function NAME_Constructor
    std::string constructorName = className->name + "_Constructor";
    llvm::Function* constructorFunc = module.getFunction(constructorName);
    if (!constructorFunc)
    {
        // Create default empty constructor
        llvm::FunctionType* constructorType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), constructorArgs,
                                                                      false);
        constructorFunc = llvm::Function::Create(constructorType, llvm::Function::ExternalLinkage, constructorName,
                                                 module);
        llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", constructorFunc);


        builder.SetInsertPoint(entry);

        // call void @A_Init(ptr %this)
        std::string initName = className->name + "_Init";
        llvm::Function* initFunc = module.getFunction(initName);
        if (!initFunc)
        {
            llvm::errs() << "Error: Init function not found for " << className->name << "\n";
            return nullptr;
        }
        std::vector<llvm::Value*> initArgs;
        initArgs.push_back(constructorFunc->getArg(0));
        builder.CreateCall(initFunc, initArgs);

        builder.CreateRetVoid();
    }

    if (extension)
    {
        // Add functions of a base class
        std::string baseClassName = extension->className->name;
        ClassEntry* baseClass = symbolTable.lookupClass(baseClassName, extension->className->span);
        auto baseClassMethods = baseClass->methods;

        ClassEntry* currentClass = symbolTable.lookupClass(className->name, className->span);
        auto currentClassMethods = symbolTable.lookupClass(className->name, className->span)->methods;

        // Add base class methods to the current class if not already present
        for (const auto& [fst, snd] : baseClassMethods)
        {
            if (currentClassMethods.find(fst) == currentClassMethods.end())
            {
                currentClass->addMethod(fst, snd);
                // Add call to the base class method
                std::string baseMethodName = baseClassName + "_" + fst.methodName;
                std::string currentMethodName = className->name + "_" + fst.methodName;
                llvm::Function* baseMethodFunc = module.getFunction(baseMethodName);
                if (!baseMethodFunc)
                {
                    llvm::errs() << "Error: Base method function not found for " << baseMethodName << "\n";
                    return nullptr;
                }
                // Create args signature
                std::vector<llvm::Type*> args;
                args.push_back(llvm::PointerType::get(classType, 0));
                for (const auto& argType : fst.parameterTypes)
                {
                    args.push_back(llvm::StructType::getTypeByName(context, argType));
                }
                llvm::Function* currentMethodFunc = llvm::Function::Create(
                    baseMethodFunc->getFunctionType(), llvm::Function::ExternalLinkage,
                    currentMethodName, module);
                llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", currentMethodFunc);
                builder.SetInsertPoint(entry);

                // Create call to the base class method
                std::vector<llvm::Value*> callArgs;
                callArgs.push_back(currentMethodFunc->getArg(0));
                for (int i = 1; i < currentMethodFunc->arg_size(); ++i)
                {
                    callArgs.push_back(currentMethodFunc->getArg(i));
                }

                if (baseMethodFunc->getFunctionType()->getReturnType()->isVoidTy())
                {
                    builder.CreateCall(baseMethodFunc, callArgs);
                    builder.CreateRetVoid();
                }
                else
                {
                    llvm::Value* ret = builder.CreateCall(baseMethodFunc, callArgs);
                    builder.CreateRet(ret);
                }
            }
        }

        // Add class fields of a base class
        auto baseClassFields = baseClass->fields;
        // baseClassFields is a vector
        for (const auto& field : baseClassFields)
        {
            if (symbolTable.getFieldIndex(className->name, field.name) == -1)
            {
                currentClass->addField(field);
            }
        }
    }
    return nullptr;
}

void MemberDeclarations::accept(Visitor& visitor)
{
    visitor.visitMemberDeclarations(*this);
}

llvm::Value* MemberDeclarations::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                         ScopedSymbolTable& symbolTable)
{
    for (const auto& memberDeclaration : member_declarations)
    {
        memberDeclaration->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

Extension::Extension(std::unique_ptr<ClassName> class_name) : className(std::move(class_name))
{
}

void Extension::accept(Visitor& visitor)
{
    visitor.visitExtension(*this);
}

llvm::Value* Extension::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                ScopedSymbolTable& symbolTable)
{
    if (className)
    {
        className->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

ClassBody::ClassBody(std::unique_ptr<MemberDeclarations> member_declarations) : memberDeclarations(
    std::move(member_declarations))
{
}

void ClassBody::accept(Visitor& visitor)
{
    visitor.visitClassBody(*this);
}

llvm::Value* ClassBody::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                ScopedSymbolTable& symbolTable)
{
    if (memberDeclarations)
    {
        memberDeclarations->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

void Expressions::accept(Visitor& visitor)
{
    visitor.visitExpressions(*this);
}


void Primary::accept(Visitor& visitor)
{
    visitor.visitPrimary(*this);
}

llvm::Value* Primary::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                              ScopedSymbolTable& symbolTable)
{
    if (literal)
    {
        return literal->codegen(context, builder, module, symbolTable);
    }
    //TODO class name
    return nullptr;
}

Primary::Primary(std::unique_ptr<ClassName>& class_name, const std::string& type, const Span& span) : class_name(
    std::move(class_name))
{
    isCompound = false;
    this->span = span;
    this->type = type;
}

Primary::Primary(std::unique_ptr<Literal>& literal, const std::string& type, const Span& span)
{
    this->literal = std::move(literal);
    isCompound = false;
    this->span = span;
    this->type = type;
}

CompoundExpression::CompoundExpression(std::string id, const Span& span, std::unique_ptr<Arguments> args,
                                       std::vector<std::unique_ptr<CompoundExpression>>
                                       compExpr) : identifier(std::move(id)), arguments(std::move(args)),
                                                   compoundExpressions(std::move(compExpr))
{
    statementType = COMPOUND_STATEMENT;
    isCompound = true;
    this->span = span;
}

void CompoundExpression::accept(Visitor& visitor)
{
    visitor.visitCompoundExpression(*this);
}

llvm::Value* CompoundExpression::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                         ScopedSymbolTable& symbolTable, llvm::Value* prevValue,
                                         const std::string& prevValueType) const
{
    llvm::Value* value = nullptr;
    std::string valueType;
    const IdentifierType type = symbolTable.getIdentifierType(identifier);

    if (type == ID_VARIABLE)
    {
        llvm::Value* ptr = symbolTable.getLocalVariable(identifier);
        if (!ptr)
        {
            // Then the variable is a class field. Access variable from the 'this' argument
            // TODO maybe bug
            llvm::StructType* classType = llvm::StructType::getTypeByName(context, symbolTable.currClassName);
            // get this ptr as first argument of current function
            // get this pointer from arguments
            llvm::Value* thisPtr = symbolTable.getThisPointer();
            int memberIndex = symbolTable.getFieldIndex(symbolTable.currClassName, identifier);

            ptr = builder.CreateConstInBoundsGEP2_32(classType, thisPtr, memberIndex, 0, identifier);

            // Create load
            ptr = builder.CreateLoad(thisPtr->getType(), ptr, identifier);
        }

        value = ptr;
        valueType = symbolTable.getIdentifierStringType(identifier, Span(0, 0, 0));
    }
    if (type == ID_CLASS)
    {
        llvm::StructType* classType = llvm::StructType::getTypeByName(context, identifier);
        //        llvm::AllocaInst *classAlloc = builder.CreateAlloca(classType, nullptr, identifier + "_object");
        llvm::DataLayout dataLayout(&module);
        uint64_t allocSize = dataLayout.getTypeAllocSize(classType);
        llvm::Value* sizeValue = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), allocSize);
        llvm::FunctionType* mallocType = llvm::FunctionType::get(llvm::Type::getInt8Ty(context)->getPointerTo(),
                                                                 {llvm::Type::getInt64Ty(context)}, false);
        llvm::FunctionCallee mallocFunc = module.getOrInsertFunction("malloc", mallocType);

        // Call malloc with the computed size
        llvm::Value* mallocCall = builder.CreateCall(mallocFunc, sizeValue, "mallocCall");

        // Optionally cast the result to the desired pointer type
        llvm::Value* typedPtr = builder.CreateBitCast(mallocCall, classType->getPointerTo(), "typedPtr");

        std::vector<llvm::Value*> constructorArgs;
        std::vector<std::string> argTypes;
        constructorArgs.push_back(typedPtr);
        if (arguments)
        {
            for (auto& arg : arguments->expressions->expressions)
            {
                llvm::Value* argValue = arg->codegen(context, builder, module, symbolTable);
                if (!argValue)
                {
                    llvm::errs() << "Error: Failed to generate code for constructor argument.\n";
                    return nullptr;
                }
                constructorArgs.push_back(argValue);
                argTypes.push_back(arg->get_type(symbolTable, symbolTable.currClassName));
            }
        }

        std::string constructorName = identifier + +"_Constructor";
        for (const auto& argType : argTypes)
        {
            constructorName += "_" + argType;
        }
        llvm::Function* constructorFunc = module.getFunction(constructorName);
        if (!constructorFunc)
        {
            llvm::errs() << "Error: " << constructorName << " not found.\n";
            return nullptr;
        }
        builder.CreateCall(constructorFunc, constructorArgs);
        value = typedPtr;
        valueType = identifier;
    }
    if (type == ID_FUNCTION)
    {
        std::string funcName = identifier;
        std::string funcClassName;

        if (prevValueType.empty())
        {
            funcClassName = symbolTable.currClassName;
        }
        else
        {
            funcClassName = prevValueType;
        }
        std::vector<llvm::Value*> args;
        std::vector<std::string> argTypes;
        // TODO add pointer to the current object
        if (prevValue)
        {
            args.push_back(prevValue); // Add the initial variable as the first argument
        }
        else
        {
            args.push_back(symbolTable.getThisPointer());
        }

        // Add other arguments if available
        if (arguments)
        {
            for (auto& arg : arguments->expressions->expressions)
            {
                llvm::Value* argValue = arg->codegen(context, builder, module, symbolTable);
                auto strArgType = arg->get_type(symbolTable, symbolTable.currClassName);
                // create load
                if (argValue && argValue->getType()->isPointerTy())
                {
                    // Create load instruction to retrieve the value pointed to by argValue

                    llvm::Type* argType = llvm::StructType::getTypeByName(context, strArgType);
                    argValue = builder.CreateLoad(argType, argValue, "loaded_arg");
                }
                if (!argValue)
                {
                    llvm::errs() << "Error: Failed to generate code for function argument.\n";
                    return nullptr;
                }
                args.push_back(argValue); // Add argument value to the call list

                argTypes.push_back(strArgType);
            }
        }

        const auto* methodEntry = symbolTable.lookupFunction(funcClassName, funcName, argTypes,
                                                             Span(0, 0, 0), false);
        for (const auto& argType : methodEntry->signature.parameterTypes)
        {
            funcName += "_" + argType;
        }
        auto t = funcClassName + "_" + funcName;
        llvm::Function* function = module.getFunction(funcClassName + "_" + funcName);
        if (!function)
        {
            llvm::errs() << "Error: " << identifier << " not found.\n";
            return nullptr;
        }

        if (function->getReturnType()->isVoidTy())
        {
            // TODO add other arguments
            value = builder.CreateCall(function, args);
            valueType = "void";
        }
        else
        {
            // TODO add other arguments
            llvm::Value* callResult = builder.CreateCall(function, args, "call_" + identifier);

            // Allocate memory for the return value
            llvm::Type* returnType = function->getReturnType();
            llvm::AllocaInst* alloca = builder.CreateAlloca(returnType, nullptr, "alloca_return_val");

            // Store the call result in the allocated memory (if applicable)
            builder.CreateStore(callResult, alloca);
            value = alloca;
            valueType = function->getReturnType()->getStructName();
        }
    }
    if (compoundExpressions.empty())
    {
        return value;
    }
    return compoundExpressions[0]->codegen(context, builder, module, symbolTable, value, valueType);
}

ProgramArguments::ProgramArguments(std::unique_ptr<Literals> literals) : literals(std::move(literals))
{
}

void ProgramArguments::accept(Visitor& visitor)
{
    visitor.visitProgramArguments(*this);
}

void MemberDeclaration::accept(Visitor& visitor)
{
    visitor.visitMemberDeclaration(*this);
}

llvm::Value* MemberDeclaration::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                        ScopedSymbolTable& symbolTable)
{
    if (variableDeclaration)
    {
        variableDeclaration->codegen(context, builder, module, symbolTable);
    }
    if (methodDeclaration)
    {
        methodDeclaration->codegen(context, builder, module, symbolTable);
    }
    if (constructorDeclaration)
    {
        constructorDeclaration->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

ConstructorDeclaration::ConstructorDeclaration(std::unique_ptr<Parameters> parameters, std::unique_ptr<Body> body,
                                               const Span& span,
                                               std::string className) : parameters(std::move(parameters)),
                                                                        body(std::move(body)),
                                                                        span(span),
                                                                        className(std::move(className))
{
}

void ConstructorDeclaration::accept(Visitor& visitor)
{
    visitor.visitConstructorDeclaration(*this);
}

llvm::Value* ConstructorDeclaration::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder,
                                             llvm::Module& module, ScopedSymbolTable& symbolTable)
{
    const llvm::StructType* classType = llvm::StructType::getTypeByName(context, className);


    // Generate the constructor function (name it based on the class it's constructing).
    std::vector<llvm::Type*> paramTypes;

    paramTypes.push_back(classType->getPointerTo());

    std::vector<std::string> paramStringTypes;
    std::vector<std::string> paramNames;
    if (parameters)
    {
        for (const auto& param : parameters->parameters)
        {
            auto stringParamType = param->className->name;
            paramStringTypes.push_back(stringParamType);
            llvm::Type* paramType = llvm::StructType::getTypeByName(context, stringParamType);
            if (!paramType)
            {
                llvm::errs() << "Error: Failed to generate code for parameter " << param->name << "\n";
                return nullptr;
            }
            paramTypes.push_back(paramType);
            paramNames.push_back(param->name);
        }
    }


    llvm::FunctionType* functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), paramTypes, false);
    std::string constructorName = className + "_Constructor";
    for (const auto& paramStringType : paramStringTypes)
    {
        constructorName += "_" + paramStringType;
    }
    llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, constructorName,
                                                      &module);

    size_t idx = 0;
    for (auto& arg : function->args())
    {
        if (idx == 0)
        {
            // The first argument is 'this'
            arg.setName("this");
        }
        else
        {
            // Other parameters
            arg.setName(paramNames[idx - 1]); // Offset by 1 to account for 'this'
        }
        ++idx;
    }


    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(entryBlock);

    //Call init function
    const std::string initName = className + "_Init";
    llvm::Function* initFunc = module.getFunction(initName);
    if (!initFunc)
    {
        llvm::errs() << "Error: Init function not found for " << className << "\n";
        return nullptr;
    }
    std::vector<llvm::Value*> initArgs;
    initArgs.push_back(function->getArg(0));
    builder.CreateCall(initFunc, initArgs);
    // Step 5: Add function parameters to the symbol table

    idx = 0;
    for (auto& arg : function->args())
    {
        if (idx == 0)
        {
            idx++;
            continue;
        }
        llvm::AllocaInst* alloca = builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
        builder.CreateStore(&arg, alloca);
        std::string argName = arg.getName().str();
        symbolTable.addLocalVariable(argName, alloca, paramTypes[idx++]->getStructName().str());
    }


    llvm::Value* thisPtr = function->getArg(0);
    symbolTable.setThisPointer(thisPtr);

    if (body)
    {
        body->codegen(context, builder, module, symbolTable);
    }

    // Finally, return void from the constructor function.
    builder.CreateRetVoid();
    verifyFunction(*function);
    symbolTable.addFunctionValue(constructorName, className, paramStringTypes, function);

    return nullptr;
}

VariableDeclaration::VariableDeclaration(std::unique_ptr<VariableName> variable,
                                         std::unique_ptr<Expression> expression) : variable(std::move(variable)),
    expression(std::move(expression))
{
}

void VariableDeclaration::accept(Visitor& visitor)
{
    visitor.visitVariableDeclaration(*this);
}


llvm::Value* VariableDeclaration::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                          ScopedSymbolTable& symbolTable)
{
    if (isClassField)
    {
        // Get the current class type
        const llvm::StructType* classType = llvm::StructType::getTypeByName(context, symbolTable.currClassName);
        if (!classType)
        {
            llvm::errs() << "Error: Class type not found.\n";
            return nullptr;
        }

        // Determine the LLVM type for the variable
        llvm::Type* fieldType = llvm::StructType::getTypeByName(context, symbolTable.currClassName);

        // Initialize the field in the class constructor
        llvm::Function* constructor = module.getFunction(symbolTable.currClassName + "_Init");
        if (constructor)
        {
            // Set the insertion point to the constructor's entry block
            llvm::IRBuilder ctorBuilder(&constructor->getEntryBlock(), constructor->getEntryBlock().begin());

            // Get the 'this' pointer (first argument of the constructor)
            llvm::Argument* thisPtr = constructor->arg_begin();
            // TODO get index of the field
            const unsigned fieldIndex = symbolTable.getFieldIndex(symbolTable.currClassName, variable->name);
            if (fieldIndex == -1)
            {
                llvm::errs() << "Error: Field index not found.\n";
                return nullptr;
            }

            // Get the pointer to the new field
            llvm::Value* fieldPtr = ctorBuilder.CreateConstInBoundsGEP2_32(
                fieldType, thisPtr, fieldIndex, 0, variable->name + "_load");

            // Generate the initialization value
            llvm::Value* initValue = expression->codegen(context, ctorBuilder, module, symbolTable);

            // Store the initialization value into the field
            ctorBuilder.CreateStore(initValue, fieldPtr);
            return nullptr;
        }
    }
    if (expression->isCompound)
    {
        auto* expr = dynamic_cast<CompoundExpression*>(expression.get());
        const auto val = expr->codegen(context, builder, module, symbolTable);
        symbolTable.addLocalVariable(variable->name, val, expr->identifier);
        return val;
    }
    const auto* expr = dynamic_cast<Primary*>(expression.get());
    llvm::StructType* classType = llvm::StructType::getTypeByName(context, expr->type);
    llvm::AllocaInst* classAlloc = builder.CreateAlloca(classType, nullptr, variable->name);
    std::string constructorName = expr->type + "_Constructor_";

    std::vector<llvm::Value*> constructorArgs;
    constructorArgs.push_back(classAlloc);
    if (expr->literal)
    {
        if (expr->literal->type == BOOL_LITERAL)
        {
            const auto* boolLiteral = dynamic_cast<BoolLiteral*>(expr->literal.get());
            llvm::Value* boolValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), boolLiteral->value);
            constructorArgs.push_back(boolValue);
            constructorName += "Boolean";
        }
        else if (expr->literal->type == INT_LITERAL)
        {
            const auto* intLiteral = dynamic_cast<IntLiteral*>(expr->literal.get());
            llvm::Value* intValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), intLiteral->value);
            constructorArgs.push_back(intValue);
            constructorName += "Integer";
        }
        else if (expr->literal->type == REAL_LITERAL)
        {
            const auto* realLiteral = dynamic_cast<RealLiteral*>(expr->literal.get());
            llvm::Value* realValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context),
                                                           static_cast<double>(realLiteral->value));
            constructorArgs.push_back(realValue);
            constructorName += "Real";
        }
    }

    llvm::Function* constructorFunc = module.getFunction(constructorName);
    if (!constructorFunc)
    {
        llvm::errs() << "Error: " << constructorName << " not found.\n";
        return nullptr;
    }
    builder.CreateCall(constructorFunc, constructorArgs);
    symbolTable.addLocalVariable(variable->name, classAlloc, expr->type);
    return classAlloc;
}

MethodDeclaration::MethodDeclaration(std::unique_ptr<MethodName>& method_name, std::unique_ptr<Parameters>& parameters,
                                     std::unique_ptr<ReturnType>& return_type,
                                     std::unique_ptr<Body>& body) : methodName(std::move(method_name)),
                                                                    parameters(std::move(parameters)),
                                                                    returnType(std::move(return_type)),
                                                                    body(std::move(body))
{
}

void MethodDeclaration::accept(Visitor& visitor)
{
    visitor.visitMethodDeclaration(*this);
}

bool isReturnStatement(llvm::Value* V)
{
    // Check if the value is an instruction
    if (auto* Inst = llvm::dyn_cast<llvm::Instruction>(V))
    {
        // Check if the instruction is a ReturnInst
        if (llvm::isa<llvm::ReturnInst>(Inst))
        {
            return true;
        }
    }
    return false; // Not a return instruction
}

llvm::Value* MethodDeclaration::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                        ScopedSymbolTable& symbolTable)
{
    // Step 1: Get the function return type
    llvm::Type* llvmReturnType = nullptr;
    if (returnType == nullptr)
    {
        llvmReturnType = llvm::Type::getVoidTy(context);
    }
    else
    {
        llvmReturnType = llvm::StructType::getTypeByName(context, returnType->className->name);
    }

    if (!llvmReturnType)
    {
        llvm::errs() << "Error: Failed to generate return type for method " << methodName->name << "\n";
        return nullptr;
    }


    // Step 2: Generate function parameters
    std::vector<llvm::Type*> paramTypes;
    paramTypes.push_back(llvm::StructType::getTypeByName(context, symbolTable.currClassName)->getPointerTo());
    std::vector<std::string> paramStringTypes;
    std::vector<std::string> paramNames;
    int i = 0;
    if (parameters)
    {
        for (const auto& param : parameters->parameters)
        {
            auto stringParamType = param->className->name;
            paramStringTypes.push_back(stringParamType);
            llvm::Type* paramType = llvm::StructType::getTypeByName(context, stringParamType);
            if (!paramType)
            {
                llvm::errs() << "Error: Failed to generate code for parameter " << param->name << "\n";
                return nullptr;
            }
            paramTypes.push_back(paramType);
            paramNames.push_back(param->name);
            i++;
        }
    }
    //
    const auto* methodEntry = symbolTable.lookupFunction(symbolTable.currClassName, methodName->name, paramStringTypes,
                                                         Span(0, 0, 0), false);
    llvm::FunctionType* functionType = llvm::FunctionType::get(llvmReturnType, paramTypes, false);
    std::string funcName = symbolTable.currClassName + "_" + methodName->name;
    for (const auto& paramStringType : methodEntry->signature.parameterTypes)
    {
        funcName += "_" + paramStringType;
    }
    llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, funcName,
                                                      &module);
    //
    // Step 3: Name the function arguments
    size_t idx = 0;
    for (auto& arg : function->args())
    {
        if (idx == 0)
        {
            arg.setName("this");
            idx++;
            continue;
        }
        arg.setName(paramNames[idx - 1]);
        idx++;
    }

    //    // Step 4: Create a new basic block to start inserting into
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(entryBlock);
    // Step 5: Add function parameters to the symbol table
    idx = 0;
    for (auto& arg : function->args())
    {
        if (idx == 0)
        {
            idx++;
            continue;
        }
        llvm::AllocaInst* alloca = builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
        builder.CreateStore(&arg, alloca);
        symbolTable.addLocalVariable(paramNames[idx - 1], alloca, paramTypes[idx]->getStructName().str());
        idx++;
    }

    llvm::Value* thisPtr = function->getArg(0);
    symbolTable.setThisPointer(thisPtr);
    // Step 6: Generate code for the function body
    auto retValue = body->codegen(context, builder, module, symbolTable);
    // Step 7: Return the generated value (for non-void functions)

    if (llvmReturnType->isVoidTy())
    {
        builder.CreateRetVoid();
    }

    verifyFunction(*function);

    symbolTable.addFunctionValue(methodName->name, symbolTable.currClassName, paramStringTypes, function);
    return function;
}


void Parameters::accept(Visitor& visitor)
{
    visitor.visitParameters(*this);
}

MethodName::MethodName(std::string name, const Span& span) : name(std::move(name)), span(span)
{
}

void MethodName::accept(Visitor& visitor)
{
    visitor.visitMethodName(*this);
}

Parameter::Parameter(std::string name, const Span& span, std::unique_ptr<ClassName> class_name) : name(std::move(name)),
    span(span), className(
        std::move(class_name))
{
}

void Parameter::accept(Visitor& visitor)
{
    visitor.visitParameter(*this);
}

VariableName::VariableName(std::string name, const Span& span) : name(std::move(name)), span(span)
{
}

void VariableName::accept(Visitor& visitor)
{
    visitor.visitVariableName(*this);
}

ReturnType::ReturnType(std::unique_ptr<ClassName> class_name) : className(std::move(class_name))
{
}

void ReturnType::accept(Visitor& visitor)
{
    visitor.visitReturnType(*this);
}

BodyDeclaration::BodyDeclaration(std::unique_ptr<VariableDeclaration> variable_declaration) : variableDeclaration(
    std::move(variable_declaration))
{
}

BodyDeclaration::BodyDeclaration(std::unique_ptr<Statement> statement) : statement(std::move(statement))
{
}

void BodyDeclaration::accept(Visitor& visitor)
{
    visitor.visitBodyDeclaration(*this);
}

llvm::Value* BodyDeclaration::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                      ScopedSymbolTable& symbolTable)
{
    if (variableDeclaration)
    {
        return variableDeclaration->codegen(context, builder, module, symbolTable);
    }
    if (statement)
    {
        return statement->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

void Statement::accept(Visitor& visitor)
{
    visitor.visitStatement(*this);
}


void Assignment::accept(Visitor& visitor)
{
    visitor.visitAssignment(*this);
}

llvm::Value* Assignment::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                 ScopedSymbolTable& symbolTable)
{
    if (expression->isCompound)
    {
        llvm::Value* value = expression->codegen(context, builder, module, symbolTable);
        if (!value)
        {
            llvm::errs() << "Error: Failed to generate code for compound assignment expression.\n";
            return nullptr;
        }

        // Get the destination pointer for the assignment
        llvm::Value* ptr = symbolTable.getLocalVariable(variableName->name);

        if (symbolTable.getFieldIndex(symbolTable.currClassName, variableName->name) != -1)
        {
            llvm::Value* thisPtr = symbolTable.getThisPointer();
            // Then the variable is a class field. Access variable from the 'this' argument
            // TODO maybe bug
            llvm::StructType* classType = llvm::StructType::getTypeByName(context, symbolTable.currClassName);
            // get this ptr as first argument of current function
            // get this pointer from arguments

            const int memberIndex = symbolTable.getFieldIndex(symbolTable.currClassName, variableName->name);

            ptr = builder.CreateConstInBoundsGEP2_32(classType, thisPtr, memberIndex, 0, variableName->name);

            // Create load
            ptr = builder.CreateLoad(thisPtr->getType(), ptr, variableName->name);
        }

        if (!ptr)
        {
            llvm::errs() << "Error: Undefined variable " << variableName->name << "\n";
            return nullptr;
        }

        // Use memcpy to copy the value
        llvm::Type* valueType = value->getType();
        const llvm::DataLayout dataLayout(&module); // Get the data layout from the module
        const uint64_t size = dataLayout.getTypeAllocSize(valueType); // Get the size of the type

        // Create constants for memcpy parameters
        llvm::Value* sizeValue = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), size);
        llvm::Value* isVolatile = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0);

        // Call llvm.memcpy intrinsic
        // TODO maybe memmove
        llvm::Function* memcpyFunc = getDeclaration(&module, llvm::Intrinsic::memcpy,
                                                    {
                                                        value->getType(), ptr->getType(),
                                                        sizeValue->getType()
                                                    }
        );
        builder.CreateCall(memcpyFunc, {ptr, value, sizeValue, isVolatile});

        return value;
    }
    llvm::Value* value = expression->codegen(context, builder, module, symbolTable);
    if (!value)
    {
        llvm::errs() << "Error: Failed to generate code for assignment expression.\n";
        return nullptr;
    }
    llvm::Value* ptr = symbolTable.getLocalVariable(variableName->name);
    if (!ptr)
    {
        llvm::errs() << "Error: Undefined local variable " << variableName->name << "\n";
        return nullptr;
    }
    builder.CreateStore(value, ptr);
    return value;
}

llvm::Value* IfStatement::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                  ScopedSymbolTable& symbolTable)
{
    llvm::Value* pointerToBoolean = expression->codegen(context, builder, module, symbolTable);
    llvm::Value* valuePtr = builder.CreateStructGEP(

        llvm::StructType::getTypeByName(context, "Boolean"),
        pointerToBoolean,
        0,
        "boolFieldPtr"
    );
    llvm::Value* CondV = builder.CreateLoad(
        llvm::Type::getInt1Ty(context), // The type of the value.
        valuePtr,
        "loadBoolValue"
    );

    // Convert the condition to a boolean value (assume nonzero is true)
    CondV = builder.CreateICmpNE(CondV, llvm::ConstantInt::get(CondV->getType(), 0), "ifcond");

    // Create basic blocks for 'then', 'else', and 'merge'
    llvm::Function* TheFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(context, "btrue", TheFunction);
    llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(context, "bfalse", TheFunction); // Add TheFunction here
    llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(context, "end", TheFunction); // Add TheFunction here

    // Create the conditional branch
    builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // Generate code for the 'then' block
    builder.SetInsertPoint(ThenBB);
    llvm::Value* ThenV = this->ifBranch->codegen(context, builder, module, symbolTable);

    builder.CreateBr(MergeBB);

    ThenBB = builder.GetInsertBlock();

    builder.SetInsertPoint(ElseBB);
    const llvm::Value* ElseV = nullptr;
    if (this->elseBranch)
    {
        ElseV = this->elseBranch->codegen(context, builder, module, symbolTable);
    }
    builder.CreateBr(MergeBB);

    builder.SetInsertPoint(MergeBB);
    return nullptr;
}

WhileLoop::WhileLoop(std::unique_ptr<Expression> expression, std::unique_ptr<Body> body) : expression(std::move(
        expression)),
    body(std::move(body))
{
    statementType = WHILE_LOOP;
}

void WhileLoop::accept(Visitor& visitor)
{
    visitor.visitWhileLoop(*this);
}

llvm::Value* WhileLoop::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                ScopedSymbolTable& symbolTable)
{
    llvm::Function* TheFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* LoopBB = llvm::BasicBlock::Create(context, "loop", TheFunction);
    llvm::BasicBlock* AfterBB = llvm::BasicBlock::Create(context, "afterloop", TheFunction);
    llvm::Value* pointerToBoolean = expression->codegen(context, builder, module, symbolTable);
    llvm::Value* valuePtr = builder.CreateStructGEP(

        llvm::StructType::getTypeByName(context, "Boolean"),
        pointerToBoolean,
        0,
        "boolFieldPtr"
    );
    llvm::Value* CondV = builder.CreateLoad(
        llvm::Type::getInt1Ty(context), // The type of the value.
        valuePtr,
        "loadBoolValue"
    );
    if (!CondV)
        return nullptr;
    CondV = builder.CreateICmpNE(CondV, llvm::ConstantInt::get(CondV->getType(), 0), "whilecond");
    builder.CreateCondBr(CondV, LoopBB, AfterBB);
    builder.SetInsertPoint(LoopBB);
    if (body)
    {
        body->codegen(context, builder, module, symbolTable);
    }

    pointerToBoolean = expression->codegen(context, builder, module, symbolTable);
    valuePtr = builder.CreateStructGEP(
        llvm::StructType::getTypeByName(context,
                                        "Boolean"), // This retrieves the type definition of the 'Boolean' class.
        pointerToBoolean, // Pointer to the object instance.
        0, // Field index: assuming value is at index 0.
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
                                                                    elseBranch(std::move(else_branch))
{
    statementType = IF_STATEMENT;
}

void IfStatement::accept(Visitor& visitor)
{
    visitor.visitIfStatement(*this);
}

IfBranch::IfBranch(std::unique_ptr<Body> body) : body(std::move(body))
{
}

void IfBranch::accept(Visitor& visitor)
{
    visitor.visitIfBranch(*this);
}

llvm::Value* IfBranch::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                               ScopedSymbolTable& symbolTable)
{
    if (body)
    {
        return body->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

ElseBranch::ElseBranch(std::unique_ptr<Body> body) : body(std::move(body))
{
}

void ElseBranch::accept(Visitor& visitor)
{
    visitor.visitElseBranch(*this);
}

llvm::Value* ElseBranch::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                 ScopedSymbolTable& symbolTable)
{
    if (body)
    {
        body->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression))
{
    statementType = RETURN_STATEMENT;
}

void ReturnStatement::accept(Visitor& visitor)
{
    visitor.visitReturnStatement(*this);
}

llvm::Value* ReturnStatement::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                      ScopedSymbolTable& symbolTable)
{
    llvm::BasicBlock* currentBlock = builder.GetInsertBlock();
    const llvm::Function* currentFunction = currentBlock->getParent();
    const auto functionReturnType = currentFunction->getReturnType();
    auto* bodyVal = expression->codegen(context, builder, module, symbolTable);
    if (expression)
    {
        if (functionReturnType->isVoidTy())
        {
            builder.CreateRetVoid();
        }
        else
        {
            if (bodyVal->getType()->isPointerTy())
            {
                bodyVal = builder.CreateLoad(functionReturnType, bodyVal, "returnVal");
            }
            builder.CreateRet(bodyVal);
        }
        return bodyVal;
    }
    return nullptr;
}

Body::Body(std::unique_ptr<BodyDeclarations> body_declarations) : bodyDeclarations(std::move(body_declarations))
{
}

void Body::accept(Visitor& visitor)
{
    visitor.visitBody(*this);
}

llvm::Value* Body::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                           ScopedSymbolTable& symbolTable)
{
    if (bodyDeclarations)
    {
        return bodyDeclarations->codegen(context, builder, module, symbolTable);
    }
    return nullptr;
}

BodyDeclarations::BodyDeclarations(std::vector<std::unique_ptr<BodyDeclaration>> body_declarations) : bodyDeclarations(
    std::move(body_declarations))
{
}

void BodyDeclarations::accept(Visitor& visitor)
{
    visitor.visitBodyDeclarations(*this);
}

llvm::Value* BodyDeclarations::codegen(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module,
                                       ScopedSymbolTable& symbolTable)
{
    llvm::Value* value = nullptr;
    for (const auto& bodyDeclaration : bodyDeclarations)
    {
        value = bodyDeclaration->codegen(context, builder, module, symbolTable);
    }
    return value;
}

std::string Expression::get_type(ScopedSymbolTable& symbolTable, std::string previousType)
{
    if (!isCompound)
    {
        return type;
    }

    auto* expr = dynamic_cast<CompoundExpression*>(this);

    std::vector<std::string> argTypes;
    if (expr->arguments)
    {
        for (const auto& arg : expr->arguments->expressions->expressions)
        {
            if (arg->isCompound)
            {
                const auto compoundArgument = dynamic_cast<CompoundExpression*>(arg.get());
                compoundArgument->get_type(symbolTable, symbolTable.currClassName);
            }
            argTypes.push_back(arg->get_type(symbolTable, symbolTable.currClassName));
        }
    }

    const auto identifierType = symbolTable.getIdentifierType(expr->identifier);
    if (identifierType == ID_VARIABLE)
    {
        previousType = symbolTable.lookupVariable(expr->identifier, expr->span)->type;
        symbolTable.makeVariableUsed(expr->identifier);
    }
    else if (identifierType == ID_CLASS)
    {
        previousType = expr->identifier;
        if (expr->arguments)
        {
            symbolTable.lookupFunction(previousType, "Constructor", argTypes, expr->span);
        }
        //llvmSymbolTable.lookupClass(expr->identifier, expr->span);
    }
    else if (identifierType == ID_FUNCTION)
    {
        previousType = symbolTable.lookupFunction(
            previousType, expr->identifier, argTypes, expr->span
        )->returnType;

        //llvmSymbolTable.lookupFunction(expr->identifier, expr->span);
    }
    else
    {
        throw std::runtime_error(
            "Identifier " + expr->identifier + " is not declared" +
            " at line: " + std::to_string(expr->span.get_line_num()) +
            " column: " + std::to_string(expr->span.get_pos_begin())
        );
    }

    const std::string originalPreviousType = previousType;
    for (const auto& compoundExpression : expr->compoundExpressions)
    {
        previousType = compoundExpression->get_type(symbolTable, originalPreviousType);
    }

    return previousType;
}
