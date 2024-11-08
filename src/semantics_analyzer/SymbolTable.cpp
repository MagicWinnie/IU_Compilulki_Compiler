#include <iostream>
#include "SymbolTable.h"


void SymbolTable::addVariableEntry(const std::string& name, const std::string& type, const bool is_constant)
{
    varEntries[name] = {name, type, is_constant};
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



void ScopedSymbolTable::enterScope()
{
    scopes.emplace_back();
}

void ScopedSymbolTable::leaveScope()
{
    if (!scopes.empty())
    {
        // Find all unused variables in the current scope
        auto& current_scope = scopes.back();
        for (const auto& [name, entry] : current_scope.varEntries)
        {
            if (!entry.is_used)
            {
                unusedVariables.insert(name);
            }
        }

        scopes.pop_back();
    }
}

void ScopedSymbolTable::addVariableEntry(const std::string& name, const std::string& type, const Span& span,
                                         const bool is_constant)
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
        identifierStringTypes[name] = type;
        current_scope.addVariableEntry(name, type, is_constant);
    }
}

void ScopedSymbolTable::addFunctionEntry(const std::string& name, const std::string& className, const std::string& returnType, const Span& span,
                                         const std::vector<std::string>& paramTypes) {
    MethodSignature signature(name, paramTypes);
    ClassEntry* classEntry = lookupClass(className, span, true);
    identifierTypes[name] = ID_FUNCTION;
    identifierStringTypes[name] = returnType;
    classEntry->addMethod(signature, {signature, returnType});
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

const VariableEntry* ScopedSymbolTable::lookupVariable(const std::string& name, const Span& span,
                                                       const bool throw_error)
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        const auto& [varEntries] = *it;
        auto found = varEntries.find(name);
        if (found != varEntries.end())
        {
            return &found->second;
        }
    }
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
    for (auto it = scopes.size(); it-- > 0; )
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

MethodEntry* ScopedSymbolTable::lookupFunction(const std::string& className, const std::string& methodName,
                                                     const std::vector<std::string>& params,
                                                     const Span& span, const bool throw_error)
{
    MethodSignature signature( methodName, params);
    ClassEntry* classEntry = lookupClass(className, span, true);
    MethodEntry* methodEntry = classEntry->lookupMethod(signature);
    if (methodEntry)
    {
        return methodEntry;
    }

    if (throw_error)
    {
        throw std::runtime_error(
            "Method '" + methodName + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    return nullptr;
}

bool ScopedSymbolTable::doesMethodExists(std::string& name, std::string& className)
{
   ClassEntry classEntry = classEntries[className];
   return classEntry.doesMethodExists(className);
}


ClassEntry* ScopedSymbolTable::lookupClass(const std::string& name, const Span& span,
                                                 const bool throw_error)
{
   ClassEntry* classEntry = nullptr;
   if(classEntries.find(name) != classEntries.end()){
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

IdentifierType ScopedSymbolTable::getIdentifierType(std::string identifier) {
   if(identifierTypes.find(identifier) != identifierTypes.end()){
         return identifierTypes[identifier];
   }
    return ID_UNDEFINED;
}

llvm::Value *ScopedSymbolTable::getLocalVariable(std::string varName) {
    return varEntries[varName];
}

void ScopedSymbolTable::addLocalVariable(std::string &varName, llvm::Value *pInst, const std::string & type) {
    varEntries[varName] = pInst;
}

std::string ScopedSymbolTable::getIdentifierStringType(std::string identifier){
    return identifierStringTypes[identifier];
}
