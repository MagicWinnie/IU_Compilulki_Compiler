//
// Created by nai1ka on 10/23/24.
//
#pragma once

#include <utility>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <llvm/IR/Value.h>

#include "../lexical_analyzer/span.h"

enum IdentifierType
{
    ID_VARIABLE,
    ID_FUNCTION,
    ID_CLASS,
    ID_UNDEFINED
};

struct VariableEntry
{
    std::string name; // Variable name
    std::string type; // Variable statementType
    bool is_used = false; // Is it used?

    VariableEntry() = default;

    VariableEntry(std::string name, std::string type);

    VariableEntry(std::string name, std::string type, bool is_used);
};

struct MethodSignature
{
    std::string methodName;
    std::vector<std::string> parameterTypes; // Vector of parameter types, e.g., {"Integer", "Integer"}

    // Constructor
    MethodSignature() = default; // Default constructor
    MethodSignature(std::string methodName, std::vector<std::string> parameterTypes);

    // Define equality operator to use MethodSignature in a map or set
    bool operator==(const MethodSignature& other) const;
};

struct MethodSignatureHash
{
    std::size_t operator()(const MethodSignature& signature) const;
};

struct MethodEntry
{
    MethodSignature signature; // Signature
    std::string returnType; // Return statementType
};

class ClassEntry
{
public:
    std::string name;
    std::vector<VariableEntry> fields;
    std::unordered_map<MethodSignature, MethodEntry, MethodSignatureHash> methods;
    std::unordered_map<MethodSignature, llvm::Function*, MethodSignatureHash> methodValues;
    std::unordered_map<std::string, std::string> methodReturnTypes;
    ClassEntry* parentClass = nullptr;

    explicit ClassEntry(std::string className);

    ClassEntry() = default;

    std::string getName() const;

    void addField(const VariableEntry& field);

    std::vector<VariableEntry> getFields() const;

    void addMethod(const MethodSignature& signature, const MethodEntry& method);

    void addMethodValue(const MethodSignature& signature, llvm::Function* value);

    llvm::Function* getMethodValue(const MethodSignature& signature);

    llvm::Function* getMethodValue(const std::string& funcName, const std::vector<std::string>& argTypes);

    bool doesMethodExists(const std::string& name);

    std::vector<MethodEntry> getMethods(const std::string& name);

    std::vector<MethodEntry> getMethodsByNameWithoutTypes(const std::string& prefix);

    void setParentClass(ClassEntry* parent);

    ClassEntry* getParentClass() const;

    std::string getMethodReturnType(const std::string& name);

    const VariableEntry* lookupField(const std::string& name) const;

    MethodEntry* lookupMethod(const MethodSignature& signature);

    int getFieldIndex(const std::string& varName) const;
};


class SymbolTable
{
public:
    std::unordered_map<std::string, VariableEntry> varEntries;

    // Add an entry
    void addVariableEntry(const std::string&, const std::string&);

    // Lookup an entry
    VariableEntry* lookupVariable(const std::string&);

    std::vector<std::string> getUnusedVariables();
};

class ScopedSymbolTable
{
public:
    std::vector<SymbolTable> scopes;
    std::unordered_map<std::string, ClassEntry> classEntries;
    std::unordered_map<std::string, IdentifierType> identifierTypes;
    std::unordered_map<std::string, llvm::Value*> varEntries;
    llvm::Value* thisPtr = nullptr;

    std::string currClassName;

    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope();

    // Leave a scope (pop the current symbol table)
    void leaveScope();

    // Add an entry in the current scope
    void addVariableEntry(const std::string& name, const std::string& type, const Span& span,
                          bool isClassField = false);


    void addClassEntry(const std::string& name, const Span& span);

    bool doesMethodExists(std::string& name, const std::string& className);

    // Lookup an entry across all scopes (from innermost to outermost)
    std::unique_ptr<VariableEntry> lookupVariable(const std::string& name, const Span& span, bool throw_error = true);

    void makeVariableUsed(const std::string& name);

    MethodEntry* lookupFunction(const std::string& className, const std::string& methodName,
                                const std::vector<std::string>& params,
                                const Span&, bool throw_error = true);

    ClassEntry* lookupClass(const std::string& name, const Span& span = Span(0, 0, 0), bool throw_error = true);


    void addFunctionEntry(const std::string& name, const std::string& className, const std::string& returnType,
                          const Span& span,
                          const std::vector<std::string>& paramTypes);

    IdentifierType getIdentifierType(const std::string& identifier);

    llvm::Value* getLocalVariable(const std::string& varName);

    void addLocalVariable(const std::string& varName, llvm::Value* pInst, const std::string& type);

    std::string getIdentifierStringType(const std::string& identifier, const Span& span);

    std::string getFunctionType(const std::string& name, const std::string& className);

    llvm::Function* getMethodValue(const std::string& className, const std::string& funcName,
                                   const std::vector<std::string>& argTypes);

    void addFunctionValue(const std::string& name, const std::string& className,
                          const std::vector<std::string>& argTypes,
                          llvm::Function* func);

    int getFieldIndex(const std::string& className, const std::string& varName);

    llvm::Value* getThisPointer() const;

    void setThisPointer(llvm::Value* thisPtr);

    SymbolTable getCurrentScope();

    void changeVariableType(std::string string, const std::string basicString);
};
