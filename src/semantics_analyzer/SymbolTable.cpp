#include <iostream>
#include "SymbolTable.h"

void SymbolTable::addVariableEntry(const std::string& name, const std::string& type, bool is_constant)
{
    varEntries[name] = {name, type, is_constant};
}

void SymbolTable::addFunctionEntry(const std::string& name, const std::string& returnType,
                                   const std::vector<std::string>& paramTypes)
{
    funcEntries[name] = {name, returnType, paramTypes};
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

FunctionEntry* SymbolTable::lookupFunction(const std::string& name)
{
    const auto it = funcEntries.find(name);
    if (it != funcEntries.end())
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
        scopes.pop_back();
    }
}

void ScopedSymbolTable::addVariableEntry(const std::string& name, const std::string& type, const bool is_constant)
{
    if (!scopes.empty())
    {
        // Use a reference to modify the actual scope on the stack
        auto& current_scope = scopes.back();

        if (current_scope.varEntries.find(name) != current_scope.varEntries.end())
        {
            throw std::runtime_error("Variable '" + name + "' is already declared in this scope.");
        }

        // Add the new entry to the current scope
        current_scope.addVariableEntry(name, type, is_constant);
    }
}

void ScopedSymbolTable::addFunctionEntry(const std::string& name, const std::string& returnType,
                                         const std::vector<std::string>& paramTypes)
{
    if (!scopes.empty())
    {
        // Use a reference to modify the actual scope on the stack
        auto& current_scope = scopes.back();

        if (current_scope.funcEntries.find(name) != current_scope.funcEntries.end())
        {
            throw std::runtime_error("Function '" + name + "' is already declared in this scope.");
        }
        // Add the new entry to the current scope
        current_scope.addFunctionEntry(name, returnType, paramTypes);
    }
}

std::string ScopedSymbolTable::lookupVariable(const std::string& name, const Span& span) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        const auto& scope = *it;
        auto found = scope.varEntries.find(name);
        if (found != scope.varEntries.end())
        {
            return found->second.type;
        }
    }
    throw std::runtime_error(
        "Variable '" + name + "' used before declaration " +
        "at line: " + std::to_string(span.get_line_num()) +
        " column: " + std::to_string(span.get_pos_begin())
    );
}

FunctionEntry ScopedSymbolTable::lookupFunction(const std::string& name, const Span& span) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        const auto& scope = *it;
        auto found = scope.funcEntries.find(name);
        if (found != scope.funcEntries.end())
        {
            return found->second;
        }
    }
    throw std::runtime_error(
        "Function '" + name + "' used before declaration " +
        "at line: " + std::to_string(span.get_line_num()) +
        " column: " + std::to_string(span.get_pos_begin())
    );
}
