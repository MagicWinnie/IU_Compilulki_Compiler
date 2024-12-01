#include <iostream>
#include <utility>
#include "SymbolTable.h"


void SymbolTable::addVariableEntry(const std::string& name, const std::string& type)
{
    varEntries[name] = {name, type};
}


VariableEntry* SymbolTable::lookupVariable(const std::string& name)
{
    const auto it = varEntries.find(name);
    if (it != varEntries.end())
    {
        return &it->second;
    }
    return nullptr; // Not found
}

std::vector<std::string> SymbolTable::getUnusedVariables()
{
    // Find all unused variables in the current scope
    std::vector<std::string> unusedVariables;
    for (const auto& [name, entry] : varEntries)
    {
        if (!entry.is_used)
        {
            unusedVariables.push_back(name);
        }
    }
    return unusedVariables;
}

SymbolTable ScopedSymbolTable::getCurrentScope()
{
    return scopes.back();
}


void ScopedSymbolTable::enterScope()
{
    scopes.emplace_back();
}


void ScopedSymbolTable::leaveScope()
{
    if (!scopes.empty())
    {
        scopes.pop_back();
    }
}

void ScopedSymbolTable::addVariableEntry(const std::string& name, const std::string& type, const Span& span,
                                         bool isClassField)
{
    if (!scopes.empty())
    {
        // Use a reference to modify the actual scope on the stack
        auto& current_scope = scopes.back();

        if (current_scope.varEntries.find(name) != current_scope.varEntries.end())
        {
            throw std::runtime_error(
                "Variable '" + name + "' is already declared in this scope " +
                "at line: " + std::to_string(span.get_line_num()) +
                " column: " + std::to_string(span.get_pos_begin())
            );
        }

        // Add the new entry to the current scope
        identifierTypes[name] = ID_VARIABLE;
        variableTypes[name] = type;
        current_scope.addVariableEntry(name, type);
        const auto classEntry = lookupClass(currClassName, span, true);
        //        if(classEntry->getFieldIndex(name) != -1){
        //            throw std::runtime_error(
        //                "Variable '" + name + "' is already declared in this class " +
        //                "at line: " + std::to_string(span.get_line_num()) +
        //                " column: " + std::to_string(span.get_pos_begin())
        //            );
        //        }
        if (isClassField)
        {
            const VariableEntry field = {name, type};
            classEntry->addField(field);
        }
    }
}

void ScopedSymbolTable::addFunctionEntry(const std::string& name, const std::string& className,
                                         const std::string& returnType, const Span& span,
                                         const std::vector<std::string>& paramTypes)
{
    std::string funcName = name;
    for (const auto& paramType : paramTypes)
    {
        funcName += "_" + paramType;
    }

    const MethodSignature signature(funcName, paramTypes);
    ClassEntry* classEntry = lookupClass(className, span, true);
    identifierTypes[name] = ID_FUNCTION;

    if (classEntry->doesMethodExists(signature.methodName))
    {
        throw std::runtime_error(
            "Method '" + name + "' is already declared in this scope" +
            " at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    classEntry->addMethod(signature, {signature, returnType});
}


void ScopedSymbolTable::addFunctionValue(const std::string& name, const std::string& className,
                                         const std::vector<std::string>& argTypes, llvm::Function* func)
{
    const MethodSignature signature(name, argTypes);
    ClassEntry* classEntry = lookupClass(className, Span(0, 0, 0), true);
    classEntry->addMethodValue(signature, func);
}

void ScopedSymbolTable::addClassEntry(const std::string& name, const Span& span)
{
    if (classEntries.find(name) != classEntries.end())
    {
        throw std::runtime_error(
            "Class '" + name + "' is already declared in this scope " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    identifierTypes[name] = ID_CLASS;
    classEntries[name] = ClassEntry(name);
}

std::unique_ptr<VariableEntry> ScopedSymbolTable::lookupVariable(const std::string& name, const Span& span,
                                                                 const bool throw_error)
{
    const auto found = variableTypes.find(name);
    if (found != variableTypes.end())
    {
        return std::make_unique<VariableEntry>(name, found->second, true);
    }
    // for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    // {
    //     auto& [varEntries] = *it;
    //     const auto found = varEntries.find(name);
    //     if (found != varEntries.end())
    //     {
    //         return &found->second;
    //     }
    // }
    if (throw_error)
    {
        throw std::runtime_error(
            "Variable '" + name + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    return nullptr;
}

void ScopedSymbolTable::makeVariableUsed(const std::string& name)
{
    for (auto it = scopes.size(); it-- > 0;)
    {
        auto& varEntries = scopes[it].varEntries;
        auto found = varEntries.find(name);
        if (found != varEntries.end())
        {
            found->second.is_used = true;
            return;
        }
    }
}

std::vector<std::string> getAllAncestors(ScopedSymbolTable* symbolTable, const std::string& className)
{
    std::vector<std::string> ancestors;
    const ClassEntry* current = symbolTable->lookupClass(className, Span(0, 0, 0), false);
    while (current && current->parentClass)
    {
        ancestors.push_back(current->parentClass->name);
        current = current->parentClass;
    }
    return ancestors;
}

MethodEntry* ScopedSymbolTable::lookupFunction(const std::string& className, const std::string& methodName,
                                               const std::vector<std::string>& params,
                                               const Span& span, const bool throw_error)
{
    std::vector<std::vector<std::string>> paramOptions;
    paramOptions.emplace_back(); // Start with an empty combination

    for (const auto& param : params)
    {
        std::vector<std::string> currentTypes;
        currentTypes.push_back(param); // The type itself

        // Get all ancestors of this parameter type
        std::vector<std::string> ancestors = getAllAncestors(this, param);
        currentTypes.insert(currentTypes.end(), ancestors.begin(), ancestors.end());

        // Generate new combinations by appending each possible type
        std::vector<std::vector<std::string>> newParamOptions;
        for (const auto& existingCombination : paramOptions)
        {
            for (const auto& typeOption : currentTypes)
            {
                std::vector<std::string> newCombination = existingCombination;
                newCombination.push_back(typeOption);
                newParamOptions.push_back(newCombination);
            }
        }
        paramOptions = std::move(newParamOptions);
    }

    ClassEntry* classEntry = lookupClass(className, span, true);
    for (const auto& option : paramOptions)
    {
        // Construct the function name with the current parameter types
        std::string funcName = methodName;
        for (const auto& paramType : option)
        {
            funcName += "_" + paramType;
        }

        MethodSignature signature{funcName, option};
        MethodEntry* methodEntry = classEntry->lookupMethod(signature);
        if (methodEntry)
        {
            return methodEntry;
        }
    }

    // auto funcName = methodName;
    // for (const auto& param : params)
    // {
    //     funcName += "_" + param;
    // }
    // const MethodSignature signature(funcName, params);
    // ClassEntry* classEntry = lookupClass(className, span, true);
    // if (MethodEntry* methodEntry = classEntry->lookupMethod(signature))
    // {
    //     return methodEntry;
    // }

    if (throw_error)
    {
        std::string originalFuncName = methodName;
        for (const auto& param : params)
        {
            originalFuncName += "_" + param;
        }
        throw std::runtime_error(
            "Method '" + className + "::" + originalFuncName + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    return nullptr;
}

bool ScopedSymbolTable::doesMethodExists(std::string& name, const std::string& className)
{
    ClassEntry classEntry = classEntries[className];
    return classEntry.doesMethodExists(className);
}


ClassEntry* ScopedSymbolTable::lookupClass(const std::string& name, const Span& span,
                                           const bool throw_error)
{
    if (classEntries.find(name) != classEntries.end())
    {
        return &classEntries[name];
    }
    if (throw_error)
    {
        throw std::runtime_error(
            "Class '" + name + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    return nullptr;
}

IdentifierType ScopedSymbolTable::getIdentifierType(const std::string& identifier)
{
    if (identifierTypes.find(identifier) != identifierTypes.end())
    {
        return identifierTypes[identifier];
    }
    return ID_UNDEFINED;
}

llvm::Value* ScopedSymbolTable::getLocalVariable(const std::string& varName)
{
    return varEntries[varName];
}

void ScopedSymbolTable::addLocalVariable(const std::string& varName, llvm::Value* pInst, const std::string& type)
{
    varEntries[varName] = pInst;
}


std::string ScopedSymbolTable::getFunctionType(const std::string& name, const std::string& className)
{
    const auto classEntry = lookupClass(className, Span(0, 0, 0), false);
    return classEntry->getMethodReturnType(name);
}

std::string ScopedSymbolTable::getIdentifierStringType(const std::string& identifier, const Span& span)
{
    switch (getIdentifierType(identifier))
    {
    case ID_VARIABLE:
        return variableTypes[identifier];
    case ID_CLASS:
        return identifier;
    default:
        throw std::runtime_error(
            "Identifier " + identifier + " is not declared" +
            " at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
}

llvm::Function* ScopedSymbolTable::getMethodValue(const std::string& className, const std::string& funcName,
                                                  const std::vector<std::string>& argTypes)
{
    const auto classEntry = lookupClass(className, Span(0, 0, 0), false);
    return classEntry->getMethodValue(funcName, argTypes);
}

int ScopedSymbolTable::getFieldIndex(const std::string& className, const std::string& varName)
{
    const auto classEntry = lookupClass(className, Span(0, 0, 0), false);
    return classEntry->getFieldIndex(varName);
}

llvm::Value* ScopedSymbolTable::getThisPointer() const
{
    return thisPtr;
}

void ScopedSymbolTable::setThisPointer(llvm::Value* thisPtr)
{
    this->thisPtr = thisPtr;
}
