//
// Created by nai1ka on 10/23/24.
//
#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../lexical_analyzer/span.h"

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
    void addVariableEntry(const std::string& name, const std::string& type, bool is_constant = false);

    void addFunctionEntry(const std::string& name, const std::string& returnType,
                          const std::vector<std::string>& paramTypes);

    // Lookup an entry
    VariableEntry* lookupVariable(const std::string& name);

    FunctionEntry* lookupFunction(const std::string& name);
};

class ScopedSymbolTable
{
    std::vector<SymbolTable> scopes;

public:
    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope();

    // Leave a scope (pop the current symbol table)
    void leaveScope();

    // Add an entry in the current scope
    void addVariableEntry(const std::string& name, const std::string& type, bool is_constant = false);

    void addFunctionEntry(const std::string& name, const std::string& returnType,
                          const std::vector<std::string>& paramTypes);

    // Lookup an entry across all scopes (from innermost to outermost)
    std::string lookupVariable(const std::string&, const Span&) const;

    FunctionEntry lookupFunction(const std::string&, const Span&) const;
};
