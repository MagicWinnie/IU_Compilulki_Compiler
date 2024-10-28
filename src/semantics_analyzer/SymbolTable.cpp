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

void SymbolTable::addClassEntry(const std::string& name)
{
    classEntries[name] = {name};
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

ClassEntry* SymbolTable::lookupClass(const std::string& name)
{
    const auto it = classEntries.find(name);
    if (it != classEntries.end())
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
        current_scope.addVariableEntry(name, type, is_constant);
    }
}

void ScopedSymbolTable::addFunctionEntry(const std::string& name, const std::string& returnType, const Span& span,
                                         const std::vector<std::string>& paramTypes)
{
    if (scopes.size() >= 2)
    {
        // Use a reference to modify the actual scope on the stack
        auto& current_scope = scopes[scopes.size() - 2];
        const auto foundFunc = current_scope.funcEntries.find(name);
        if (foundFunc != current_scope.funcEntries.end())
        {
            if (foundFunc->second.paramTypes == paramTypes)
            {
                throw std::runtime_error(
                    "Method '" + name + "' is already declared in this scope " +
                    "at line: " + std::to_string(span.get_line_num()) +
                    " column: " + std::to_string(span.get_pos_begin())
                );
            }
        }
        // Add the new entry to the current scope
        current_scope.addFunctionEntry(name, returnType, paramTypes);
    }
}

void ScopedSymbolTable::addClassEntry(const std::string& name, const Span& span)
{
    if (scopes.size() >= 2)
    {
        // Use a reference to modify the actual scope on the stack
        auto& current_scope = scopes[scopes.size() - 2];
        if (current_scope.classEntries.find(name) != current_scope.classEntries.end())
        {
            throw std::runtime_error(
                "Class '" + name + "' is already declared in this scope " +
                "at line: " + std::to_string(span.get_line_num()) +
                " column: " + std::to_string(span.get_pos_begin())
            );
        }
        // Add the new entry to the current scope
        current_scope.addClassEntry(name);
    }
}

const VariableEntry* ScopedSymbolTable::lookupVariable(const std::string& name, const Span& span,
                                                       const bool throw_error) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        const auto& [varEntries, funcEntries, classEntries] = *it;
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
    else return nullptr;
}

void ScopedSymbolTable::makeVariableUsed(const std::string& name)
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        auto& [varEntries, funcEntries, classEntries] = *it;
        auto found = varEntries.find(name);
        if (found != varEntries.end())
        {
            found->second.is_used = true;
            return;
        }
    }
}

const FunctionEntry* ScopedSymbolTable::lookupFunction(const std::string& name, const Span& span,
                                                       bool throw_error) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        const auto& [varEntries, funcEntries, classEntries] = *it;
        auto found = funcEntries.find(name);
        if (found != funcEntries.end())
        {
            return &found->second;
        }
    }
    if (throw_error)
    {
        throw std::runtime_error(
            "Function '" + name + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    else
    {
        return nullptr;
    }
}

const ClassEntry* ScopedSymbolTable::lookupClass(const std::string& name, const Span& span,
                                                 bool throw_error ) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        const auto& [varEntries, funcEntries, classEntries] = *it;
        auto found = classEntries.find(name);
        if (found != classEntries.end())
        {
            return &found->second;
        }
    }
    if (throw_error)
    {
        throw std::runtime_error(
            "Class '" + name + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    else
    {
        return nullptr;
    }
}
