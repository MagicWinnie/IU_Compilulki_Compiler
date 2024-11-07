//
// Created by Nail Minnemullin on 06.11.2024.
//

#include "vector"
#include "unordered_map"
#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>


#ifndef IU_COMPILULKI_COMPILER_LLVMSYMBOLTABLE_H
#define IU_COMPILULKI_COMPILER_LLVMSYMBOLTABLE_H

enum IdentifierType {
    VARIABLE,
    FUNCTION,
    CLASS
};

class LLVMSymbolTable{
public:
    std::unordered_map<std::string, llvm::Value*> varEntries;
    std::unordered_map<std::string, std::string> varTypes;
    std::unordered_map<std::string, IdentifierType> identifierTypes;

    void addVariableEntry(const std::string&, llvm::Value*, const std::string&);


    llvm::Value* getVariableValue(const std::string&);
};



class LLVMScopedSymbolTable {
    public:
        std::vector<LLVMSymbolTable> scopes;


    // Enter a new scope (push a new symbol table onto the stack)
        void enterScope();

        // Leave a scope (pop the current symbol table)
        void leaveScope();



    llvm::Value *getLocalVariable(std::string basicString);

    std::string getLocalVariableType(std::string basicString);

    void addLocalVariable(std::string& basicString, llvm::Value *pInst, const std::string &);

    void addIdentifier(const std::string& identifier, IdentifierType type);

    IdentifierType getIdentifierType(const std::string& identifier);
};



#endif //IU_COMPILULKI_COMPILER_LLVMSYMBOLTABLE_H
