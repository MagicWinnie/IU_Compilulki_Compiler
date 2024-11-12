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

enum IdentifierType {
    ID_VARIABLE,
    ID_FUNCTION,
    ID_CLASS,
    ID_UNDEFINED
};

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
    MethodSignature() = default; // Default constructor
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
struct MethodSignatureHash
{
    std::size_t operator()(const MethodSignature& signature) const
    {
        std::size_t hash = std::hash<std::string>{}(signature.methodName);
        for (const auto& paramType : signature.parameterTypes)
        {
            hash ^= std::hash<std::string>{}(paramType) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
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
    std::unordered_map<MethodSignature, MethodEntry, MethodSignatureHash> methods;
    std::unordered_map<std::string, std::string> methoReturnTypes;
    ClassEntry* parentClass = nullptr;

public:
    ClassEntry(const std::string& className) : name(className) {}
    ClassEntry() = default;

    void addField(const VariableEntry& field)
    {
        fields.push_back(field);
    }

    void addMethod(MethodSignature signature, MethodEntry method)
    {
        methods[signature] = method;
        methoReturnTypes[signature.methodName] = method.returnType;
    }

    bool doesMethodExists(std::string& name)
    {
      std::vector<MethodEntry> classMethods = getMethods(name);
      for (const auto& method : classMethods)
      {
          if(method.signature.methodName == name)
          {
              return true;
          }
      }
        return false;
    }

    std::vector<MethodEntry> getMethods(std::string& name)
    {
        std::vector<MethodEntry> methodsList;
        for (const auto& [signature, method] : methods)
        {
            if (signature.methodName == name)
            {
                methodsList.push_back(method);
            }
        }
        return methodsList;
    }


    void setParentClass(ClassEntry* parent)
    {
        parentClass = parent;
    }

    std::string getMethodReturnType(const std::string& name)
    {
        return methoReturnTypes[this->name+"_"+name];
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

    MethodEntry* lookupMethod(MethodSignature signature)
    {
        auto it = methods.find(signature);
        if(it != methods.end())
        {
            return &it->second;
        }

        if (parentClass)
        {
            return parentClass->lookupMethod(signature);
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
    std::unordered_map<std::string, IdentifierType> identifierTypes;
    std::unordered_map<std::string, std::string> variableTypes;
    std::unordered_map<std::string, llvm::Value*> varEntries;

    std::string currClassName;

    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope();

    // Leave a scope (pop the current symbol table)
    void leaveScope();

    // Add an entry in the current scope
    void addVariableEntry(const std::string&, const std::string&, const Span&, bool is_constant = false);


    void addClassEntry(const std::string&, const Span&);

    bool doesMethodExists(std::string& name, std::string& className);

    // Lookup an entry across all scopes (from innermost to outermost)
    const VariableEntry* lookupVariable(const std::string&, const Span&, bool throw_error = true);
    void makeVariableUsed(const std::string& name);

    MethodEntry* lookupFunction(const std::string& methodName, const std::string& className, const std::vector<std::string>& arguments,
                                      const Span&, bool throw_error = true);

    ClassEntry* lookupClass(const std::string&, const Span& = Span(0,0,0), bool throw_error = true);



    void addFunctionEntry(const std::string &name, const std::string &className, const std::string &returnType,
                     const Span &span,
                     const std::vector<std::string> &paramTypes);

    IdentifierType getIdentifierType(std::string identifier);

    llvm::Value *getLocalVariable(std::string basicString);

    void addLocalVariable(std::string& basicString, llvm::Value *pInst, const std::string &);

    std::string getIdentifierStringType(std::string& identifier, std::string& className);
    std::string getIdentifierStringType(std::string& identifier);

    std::string getFunctionType(const std::string& name, const std::string& className);
};
