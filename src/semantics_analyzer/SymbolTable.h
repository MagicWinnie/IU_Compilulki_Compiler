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

enum IdentifierType {
    ID_VARIABLE,
    ID_FUNCTION,
    ID_CLASS,
    ID_UNDEFINED
};

struct VariableEntry {
    std::string name; // Variable name
    std::string type; // Variable statementType
    bool is_used = false; // Is it used?
};

struct MethodSignature {
    std::string methodName;
    std::vector<std::string> parameterTypes; // Vector of parameter types, e.g., {"Integer", "Integer"}

    // Constructor
    MethodSignature() = default; // Default constructor
    MethodSignature(std::string methodName, std::vector<std::string> parameterTypes)
        : methodName(std::move(methodName)), parameterTypes(std::move(parameterTypes)) {
    }

    // Define equality operator to use MethodSignature in a map or set
    bool operator==(const MethodSignature &other) const {
        return methodName == other.methodName && parameterTypes == other.parameterTypes;
    }
};

struct MethodSignatureHash {
    std::size_t operator()(const MethodSignature &signature) const {
        std::size_t hash = std::hash<std::string>{}(signature.methodName);
        for (const auto &paramType: signature.parameterTypes) {
            hash ^= std::hash<std::string>{}(paramType) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct MethodEntry {
    MethodSignature signature; // Signature
    std::string returnType; // Return statementType
};

class ClassEntry {


public:
    std::string name;
    std::vector<VariableEntry> fields;
    std::unordered_map<MethodSignature, MethodEntry, MethodSignatureHash> methods;
    std::unordered_map<MethodSignature, llvm::Function *, MethodSignatureHash> methodValues;
    std::unordered_map<std::string, std::string> methodReturnTypes;
    ClassEntry *parentClass = nullptr;
    explicit ClassEntry(std::string className) : name(std::move(className)) {
    }

    ClassEntry() = default;

    std::string getName() const {
        return name;
    }

    void addField(const VariableEntry &field) {
        fields.push_back(field);
    }

    std::vector<VariableEntry> getFields() const {
        return fields;
    }

    void addMethod(const MethodSignature &signature, const MethodEntry &method) {
        methods[signature] = method;
        // Get method name before _
        const std::string methodName = signature.methodName.substr(0, signature.methodName.find('_'));
        methodReturnTypes[methodName] = method.returnType;
    }

    void addMethodValue(const MethodSignature &signature, llvm::Function *value) {
        methodValues[signature] = value;
    }

    llvm::Function *getMethodValue(const MethodSignature &signature) {
        return methodValues[signature];
    }

    llvm::Function *getMethodValue(const std::string &funcName, const std::vector<std::string> &argTypes) {
        const MethodSignature signature(funcName, argTypes);
        return methodValues[signature];
    }

    bool doesMethodExists(const std::string &name) {
        std::vector<MethodEntry> classMethods = getMethods(name);
        return std::any_of(
            classMethods.begin(),
            classMethods.end(),
            [name](const MethodEntry &entry) { return entry.signature.methodName == name; }
        );
    }

    std::vector<MethodEntry> getMethods(const std::string &name) {
        std::vector<MethodEntry> methodsList;
        for (const auto &[signature, method]: methods) {
            if (signature.methodName == name) {
                methodsList.push_back(method);
            }
        }
        return methodsList;
    }

    std::vector<MethodEntry> getMethodsByNameWithoutTypes(const std::string &prefix) {
        std::vector<MethodEntry> methodsList;
        for (const auto &[signature, method]: methods) {
            const std::string methodName = signature.methodName.substr(0, signature.methodName.find('_'));
            if (methodName == prefix) {
                methodsList.push_back(method);
            }
        }
        return methodsList;
    }

    void setParentClass(ClassEntry *parent) {
        parentClass = parent;
    }

    ClassEntry *getParentClass() const {
        return parentClass;
    }

    std::string getMethodReturnType(const std::string &name) {
        return methodReturnTypes[name];
    }

    const VariableEntry *lookupField(const std::string &name) const {
        for (const auto &field: fields) {
            if (field.name == name) {
                return &field;
            }
        }
        if (parentClass) {
            return parentClass->lookupField(name);
        }
        return nullptr;
    }

    MethodEntry *lookupMethod(const MethodSignature &signature) {
        const auto it = methods.find(signature);
        if (it != methods.end()) {
            return &it->second;
        }

        if (parentClass) {
            return parentClass->lookupMethod(signature);
        }
        return nullptr;
    }

    int getFieldIndex(std::string varName) {
        for (int i = 0; i < fields.size(); i++) {
            if (fields[i].name == varName) {
                return i;
            }
        }
        if (parentClass) {
            return parentClass->getFieldIndex(varName);
        }
        return -1;

    }
};


class SymbolTable {
public:
    std::unordered_map<std::string, VariableEntry> varEntries;

    // Add an entry
    void addVariableEntry(const std::string &, const std::string &);

    // Lookup an entry
    VariableEntry *lookupVariable(const std::string &);

    std::vector<std::string> getUnusedVariables();


};

class ScopedSymbolTable {
public:
    std::vector<SymbolTable> scopes;
    std::unordered_map<std::string, ClassEntry> classEntries;
    std::unordered_map<std::string, IdentifierType> identifierTypes;
    std::unordered_map<std::string, std::string> variableTypes;
    std::unordered_map<std::string, llvm::Value *> varEntries;
    llvm::Value* thisPtr = nullptr;

    std::string currClassName;

    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope();

    // Leave a scope (pop the current symbol table)
    void leaveScope();

    // Add an entry in the current scope
    void addVariableEntry(const std::string &name, const std::string &type, const Span &span, bool isClassField = false);


    void addClassEntry(const std::string &name, const Span &span);

    bool doesMethodExists(std::string &name, const std::string &className);

    // Lookup an entry across all scopes (from innermost to outermost)
    VariableEntry *lookupVariable(const std::string &name, const Span &span, bool throw_error = true);

    void makeVariableUsed(const std::string &name);

    MethodEntry *lookupFunction(const std::string &className, const std::string &methodName,
                                const std::vector<std::string> &params,
                                const Span &, bool throw_error = true);

    ClassEntry *lookupClass(const std::string &name, const Span &span = Span(0, 0, 0), bool throw_error = true);


    void addFunctionEntry(const std::string &name, const std::string &className, const std::string &returnType,
                          const Span &span,
                          const std::vector<std::string> &paramTypes);

    IdentifierType getIdentifierType(const std::string &identifier);

    llvm::Value *getLocalVariable(const std::string &varName);

    void addLocalVariable(const std::string &varName, llvm::Value *pInst, const std::string &type);

    std::string getIdentifierStringType(const std::string &identifier, const std::string &className, const Span &span);

    std::string getIdentifierStringType(const std::string &identifier, const Span &span);

    std::string getFunctionType(const std::string &name, const std::string &className);

    llvm::Function *getMethodValue(const std::string &className, const std::string &funcName,
                                   const std::vector<std::string> &argTypes);

    void addFunctionValue(const std::string &name, const std::string &className,
                          const std::vector<std::string> &argTypes,
                          llvm::Function *func);

    int getFieldIndex(const std::string& className, const std::string& varName);

    llvm::Value *getThisPointer() const;

    void setThisPointer(llvm::Value *thisPtr);

    SymbolTable getCurrentScope();
};
