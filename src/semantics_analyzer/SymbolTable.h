//
// Created by nai1ka on 10/23/24.
//
#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <llvm/IR/Value.h>

#include "../lexical_analyzer/span.h"

struct VariableEntry
{
    std::string name; // Variable name
    std::string type; // Variable statementType
    bool is_constant{}; // Is it a constant?
    bool is_used = false; // Is it used?
};

struct MethodSignature
{

    std::string methodName;
    std::vector<std::string> parameterTypes; // Vector of parameter types, e.g., {"Integer", "Integer"}

    // Constructor
    MethodSignature( std::string methodName, std::vector<std::string> parameterTypes)
        :  methodName(std::move(methodName)), parameterTypes(std::move(parameterTypes))
    {
    }

    // Define equality operator to use MethodSignature in a map or set
    bool operator==(const MethodSignature& other) const
    {
        return methodName == other.methodName && parameterTypes == other.parameterTypes;
    }
};

struct MethodEntry
{
    MethodSignature signature; // Signature
    std::string returnType; // Return statementType
};

class ClassEntry
{
    std::string name;
    std::vector<VariableEntry> fields;
    std::unordered_map<MethodSignature, MethodEntry> methods;
    ClassEntry* parentClass = nullptr;

public:
    ClassEntry(std::string name) : name(std::move(name)) {}

    void addField(const VariableEntry& field)
    {
        fields.push_back(field);
    }

    void addMethod(MethodSignature signature, MethodEntry method)
    {
        methods[signature] = method;
    }

    bool doesMethodExists(const std::string& name)
    {
        auto it = methods.find(name);
        if (it == methods.end())
        {
            return false;
        }
        return true;
    }


    void setParentClass(ClassEntry* parent)
    {
        parentClass = parent;
    }

    const VariableEntry* lookupField(const std::string& name) const
    {
        for (const auto& field : fields)
        {
            if (field.name == name)
            {
                return &field;
            }
        }
        if (parentClass)
        {
            return parentClass->lookupField(name);
        }
        return nullptr;
    }

    const MethodEntry* lookupMethod(const std::string& name, const std::vector<std::string>& params) const
    {
        auto it = methods.find({name, params});
        if (parentClass)
        {
            return parentClass->lookupMethod(name, params);
        }
        return nullptr;
    }
};


class SymbolTable
{
public:
    std::unordered_map<std::string, VariableEntry> varEntries;

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
    std::unordered_map<std::string, ClassEntry> classEntries;
    std::unordered_set<std::string> unusedVariables;
    std::string currClassName;

    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope();

    // Leave a scope (pop the current symbol table)
    void leaveScope();

    // Add an entry in the current scope
    void addVariableEntry(const std::string&, const std::string&, const Span&, bool is_constant = false);


    void addClassEntry(const std::string&, const Span&);

    bool doesMethodExists(const std::string& name, const std::string& className);

    // Lookup an entry across all scopes (from innermost to outermost)
    const VariableEntry* lookupVariable(const std::string&, const Span&, bool throw_error = true);
    void makeVariableUsed(const std::string& name);

    MethodEntry* lookupFunction(const std::string&, const std::string&, const std::vector<std::string>&,
                                      const Span&, bool throw_error = true);

    ClassEntry* lookupClass(const std::string&, const Span&, bool throw_error = true);


    void addFunctionEntry(const std::string &name, const std::string &className, const std::string &returnType,
                     const Span &span,
                     const std::vector<std::string> &paramTypes);
};
