//
// Created by nai1ka on 10/23/24.
//
#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../lexical_analyzer/span.h"

struct VariableEntry
{
    std::string name; // Variable name
    std::string type; // Variable type
    bool is_constant{}; // Is it a constant?
    bool is_used = false; // Is it used?
};

struct MethodSignature
{
    std::string className;
    std::string methodName;
    std::vector<std::string> parameterTypes; // Vector of parameter types, e.g., {"Integer", "Integer"}

    // Constructor
    MethodSignature(std::string className, std::string methodName, std::vector<std::string> parameterTypes)
        : className(std::move(className)), methodName(std::move(methodName)), parameterTypes(std::move(parameterTypes))
    {
    }

    // Define equality operator to use MethodSignature in a map or set
    bool operator==(const MethodSignature& other) const
    {
        return className == other.className && methodName == other.methodName && parameterTypes == other.parameterTypes;
    }
};

struct MethodEntry
{
    MethodSignature signature; // Signature
    std::string returnType; // Return type
};

struct ClassEntry
{
    std::string name; // Class name
};


template <>
struct std::hash<MethodSignature>
{
    std::size_t operator()(const MethodSignature& signature) const noexcept
    {
        std::size_t hashValue = std::hash<std::string>()(signature.methodName);
        for (const auto& param : signature.parameterTypes)
        {
            hashValue ^= std::hash<std::string>()(param);
        }
        return hashValue;
    }
};

class SymbolTable
{
public:
    std::unordered_map<std::string, VariableEntry> varEntries;

    std::unordered_map<std::string, ClassEntry> classEntries;

    // Add an entry
    void addVariableEntry(const std::string&, const std::string&, bool is_constant = false);

    void addClassEntry(const std::string&);

    // Lookup an entry
    VariableEntry* lookupVariable(const std::string&);

    ClassEntry* lookupClass(const std::string&);
};

class ScopedSymbolTable
{
public:
    std::vector<SymbolTable> scopes;
    std::unordered_map<MethodSignature, MethodEntry> funcEntries;
    std::unordered_set<std::string> funcNames;
    std::unordered_set<std::string> unusedVariables;
    std::string currClassName = "";

    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope();

    // Leave a scope (pop the current symbol table)
    void leaveScope();

    // Add an entry in the current scope
    void addVariableEntry(const std::string&, const std::string&, const Span&, bool is_constant = false);

    void addFunctionEntry(const std::string&, const std::string&, const Span&, const std::vector<std::string>&);

    void addClassEntry(const std::string&, const Span&);

    bool doesMethodExists(const std::string&);

    // Lookup an entry across all scopes (from innermost to outermost)
    const VariableEntry* lookupVariable(const std::string&, const Span&, bool throw_error = true) const;
    void makeVariableUsed(const std::string& name);

    const MethodEntry* lookupFunction(const std::string&, const std::vector<std::string>&, const Span&,
                                      bool throw_error = true) const;

    const ClassEntry* lookupClass(const std::string&, const Span&, bool throw_error = true) const;
};
