//
// Created by Nail Minnemullin on 08.10.2024.
//

#ifndef IU_COMPILULKI_COMPILER_NODES_H
#define IU_COMPILULKI_COMPILER_NODES_H

#include "memory"
#include "vector"

class Entity{
public:
//    virtual bool validate() = 0;
//    virtual void generate() = 0;
    virtual ~Entity() = default;
};

class ProgramNode : public Entity {
public:
    std::unique_ptr<Entity> programDeclaration;
    std::unique_ptr<Entity> classDeclarations;
};

class ProgramDeclarationNode : public Entity {
public:
    std::string className;
    std::unique_ptr<Entity> arguments;

};


class Statement : public Entity{

};

class Expression : public Entity{

};

class Block : public Entity {
    std::vector<std::unique_ptr<Statement>> statements;
public:
    // Constructor to initialize block with a list of statements
    Block(std::vector<std::unique_ptr<Statement>> stmts)
            : statements(std::move(stmts)) {}

    // Validate each statement in the block

};



class Block;

class IfStatement : public Statement{
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> trueBlock;
    std::unique_ptr<Block> falseBlock;

public:
    // Constructor to initialize the subtrees
    IfStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Block> trueBlk, std::unique_ptr<Block> falseBlk = nullptr)
    : condition(std::move(cond)), trueBlock(std::move(trueBlk)), falseBlock(std::move(falseBlk)) {}

    // Override validate method


};

#endif //IU_COMPILULKI_COMPILER_NODES_H
