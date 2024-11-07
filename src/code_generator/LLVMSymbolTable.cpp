//
// Created by Nail Minnemullin on 06.11.2024.
//

#include "LLVMSymbolTable.h"

void LLVMScopedSymbolTable::enterScope() {
    scopes.emplace_back();
}

void LLVMScopedSymbolTable::leaveScope() {
    scopes.pop_back();
}

llvm::Value *LLVMScopedSymbolTable::getLocalVariable(std::string basicString) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto value = it->getVariableValue(basicString);
        if (value) {
            return value;
        }
    }
    return nullptr;
}

void LLVMScopedSymbolTable::addLocalVariable(std::string& basicString, llvm::Value *pInst, const std::string& type) {
    scopes.back().addVariableEntry(basicString, pInst, type);
}

std::string LLVMScopedSymbolTable::getLocalVariableType(std::string basicString) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto value = it->varTypes.find(basicString);
        if (value != it->varTypes.end()) {
            return value->second;
        }
    }
    return "";
}

void LLVMScopedSymbolTable::addIdentifier(const std::string &identifier, IdentifierType type) {
    scopes.back().identifierTypes[identifier] = type;
}

IdentifierType LLVMScopedSymbolTable::getIdentifierType(const std::string &identifier) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto value = it->identifierTypes.find(identifier);
        if (value != it->identifierTypes.end()) {
            return value->second;
        }
    }
    return VARIABLE;
}




llvm::Value *LLVMSymbolTable::getVariableValue(const std::string &name) {
    auto it = varEntries.find(name);
    return (it != varEntries.end()) ? it->second : nullptr;
}

void LLVMSymbolTable::addVariableEntry(const std::string & name, llvm::Value * value, const std::string & type) {
    varEntries[name] = value;
    varTypes[name] = type;
}
