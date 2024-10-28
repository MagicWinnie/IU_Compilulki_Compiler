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

struct FunctionEntry
{
    std::string name; // Function name
    std::string returnType; // Return type
    std::vector<std::string> paramTypes; // Parameter types
};

struct ClassEntry
{
    std::string name; // Class name
};

class SymbolTable
{
public:
    std::unordered_map<std::string, VariableEntry> varEntries;
    std::unordered_map<std::string, FunctionEntry> funcEntries;
    std::unordered_map<std::string, ClassEntry> classEntries;



    // Add an entry
    void addVariableEntry(const std::string&, const std::string&, bool is_constant = false);

    void addFunctionEntry(const std::string&, const std::string&, const std::vector<std::string>&);

    void addClassEntry(const std::string&);

    // Lookup an entry
    VariableEntry* lookupVariable(const std::string&);

    FunctionEntry* lookupFunction(const std::string&);

    ClassEntry* lookupClass(const std::string&);
};

class ScopedSymbolTable
{


public:
    std::vector<SymbolTable> scopes;
    std::pmr::unordered_set<std::string> unusedVariables;
    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope();

    // Leave a scope (pop the current symbol table)
    void leaveScope();

    // Add an entry in the current scope
    void addVariableEntry(const std::string&, const std::string&, const Span&, bool is_constant = false);

    void addFunctionEntry(const std::string&, const std::string&, const Span&, const std::vector<std::string>&);

    void addClassEntry(const std::string&, const Span&);

    // Lookup an entry across all scopes (from innermost to outermost)
    const VariableEntry* lookupVariable(const std::string&, const Span&, bool throw_error = true) const;
    void makeVariableUsed(const std::string& name);

    const FunctionEntry* lookupFunction(const std::string&, const Span&, bool throw_error = true) const;

    const ClassEntry* lookupClass(const std::string&, const Span&, bool throw_error = true) const;
};
