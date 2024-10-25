//
// Created by nai1ka on 10/23/24.
//
#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

struct VariableEntry
{
    std::string name; // Variable name
    std::string type; // Variable type
    bool is_constant{}; // Is it a constant?
};

// Structure to represent a function entry
struct FunctionEntry
{
    std::string name; // Function name
    std::string returnType; // Return type
    std::vector<std::string> paramTypes; // Parameter types
};

class SymbolTable
{
public:
    std::unordered_map<std::string, VariableEntry> varEntries; // Variable entries
    std::unordered_map<std::string, FunctionEntry> funcEntries;

    // Add an entry
    void addVariableEntry(const std::string& name, const std::string& type, bool is_constant = false)
    {
        varEntries[name] = {name, type, is_constant};
    }

    void addFunctionEntry(const std::string& name, const std::string& returnType,
                          const std::vector<std::string>& paramTypes)
    {
        funcEntries[name] = {name, returnType, paramTypes};
    }

    // Lookup an entry
    VariableEntry* lookupVariable(const std::string& name)
    {
        auto it = varEntries.find(name);
        if (it != varEntries.end())
        {
            return &it->second;
        }
        return nullptr; // Not found
    }

    FunctionEntry* lookupFunction(const std::string& name)
    {
        auto it = funcEntries.find(name);
        if (it != funcEntries.end())
        {
            return &it->second;
        }
        return nullptr; // Not found
    }
};

class ScopedSymbolTable
{
private:
    std::vector<SymbolTable> scopes;

public:
    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope()
    {
        scopes.emplace_back();
    }

    // Leave a scope (pop the current symbol table)
    void leaveScope()
    {
        if (!scopes.empty())
        {
            scopes.pop_back();
        }
    }

    // Add an entry in the current scope
    void addVariableEntry(const std::string& name, const std::string& type, bool is_constant = false)
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

    void addFunctionEntry(const std::string& name, const std::string& returnType,
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

    // Lookup an entry across all scopes (from innermost to outermost)
    std::string lookupVariable(const std::string& name) const
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
        throw std::runtime_error("Variable '" + name + "' used before declaration.");
    }

    FunctionEntry lookupFunction(const std::string& name) const
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
        throw std::runtime_error("Function '" + name + "' used before declaration.");
    }
};
