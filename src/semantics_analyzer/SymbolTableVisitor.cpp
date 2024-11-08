#include <iostream>
#include <stdexcept>
#include "SymbolTableVisitor.h"

SymbolTableVisitor::SymbolTableVisitor() {
    symbolTable.enterScope();
    symbolTable.addClassEntry("Integer", Span(0, 0, 0));
    symbolTable.addClassEntry("Boolean", Span(0, 0, 0));
    symbolTable.addClassEntry("Real", Span(0, 0, 0));
    symbolTable.addClassEntry("Array", Span(0, 0, 0));
    symbolTable.addClassEntry("List", Span(0, 0, 0));
    symbolTable.addClassEntry("String", Span(0, 0, 0));

    // Integer
    symbolTable.addFunctionEntry("toReal", "Integer", "Real", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("toBoolean", "Integer", "Boolean", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("UnaryMinus", "Integer", "Integer", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("Plus", "Integer", "Integer", Span(0, 0, 0), {"Integer"});

    symbolTable.addFunctionEntry("Minus", "Integer", "Integer", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Mult", "Integer", "Integer", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Div", "Integer", "Integer", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Rem", "Integer", "Integer", Span(0, 0, 0), {"Integer"});

    symbolTable.addFunctionEntry("Less", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Less", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("LessEqual", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("LessEqual", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Greater", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Greater", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("GreaterEqual", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("GreaterEqual", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Equal", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Equal", "Integer", "Boolean", Span(0, 0, 0), {"Real"});

    symbolTable.addFunctionEntry("print", "Integer", "void", Span(0, 0, 0), {});


    // Real
    symbolTable.addFunctionEntry("toInteger", "Integer", "Integer", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("UnaryMinus", "Integer", "Real", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("Plus", "Integer", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Plus", "Integer", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Minus", "Integer", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Minus", "Integer", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Mult", "Integer", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Mult", "Integer", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Div", "Integer", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Div", "Integer", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Rem", "Integer", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Less", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Less", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("LessEqual", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("LessEqual", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Greater", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Greater", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("GreaterEqual", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("GreaterEqual", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Equal", "Integer", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Equal", "Integer", "Boolean", Span(0, 0, 0), {"Integer"});

    symbolTable.addFunctionEntry("print", "Real", "void", Span(0, 0, 0), {});

    // Boolean
    symbolTable.addFunctionEntry("Or", "Boolean", "Boolean", Span(0, 0, 0), {"Boolean"});
    symbolTable.addFunctionEntry("And", "Boolean", "Boolean", Span(0, 0, 0), {"Boolean"});
    symbolTable.addFunctionEntry("Xor", "Boolean", "Boolean", Span(0, 0, 0), {"Boolean"});
    symbolTable.addFunctionEntry("Not", "Boolean", "Boolean", Span(0, 0, 0), {});

    symbolTable.addFunctionEntry("print", "Boolean", "void", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("scan", "Boolean", "String", Span(0, 0, 0), {});

    // List
    symbolTable.addFunctionEntry("toList", "List", "List", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("Length", "List", "Integer", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("get", "List", "T", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("set", "List", "void", Span(0, 0, 0), {"Integer", "T"});

    symbolTable.addFunctionEntry("append", "List", "List", Span(0, 0, 0), {"T"});
    symbolTable.addFunctionEntry("head", "List", "T", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("tail", "List", "List", Span(0, 0, 0), {});

    symbolTable.addFunctionEntry("print", "List", "void", Span(0, 0, 0), {});
}

SymbolTableVisitor::~SymbolTableVisitor() {
    symbolTable.leaveScope();
}

ScopedSymbolTable SymbolTableVisitor::getSymbolTable() const {
    return symbolTable;
}

void SymbolTableVisitor::visitProgram(Program &node) {
    if (node.programDeclaration) node.programDeclaration->accept(*this);
    if (node.classDeclarations) node.classDeclarations->accept(*this);
    if (node.programDeclaration && node.programDeclaration->className)
        node.programDeclaration->className->accept(*this);
}

void SymbolTableVisitor::visitProgramDeclaration(ProgramDeclaration &node) {
    // if (node.className) node.className->accept(*this);
    if (node.arguments) node.arguments->accept(*this);
}

void SymbolTableVisitor::visitClassName(ClassName &node) {
    symbolTable.lookupClass(node.name, node.span);
}

void SymbolTableVisitor::visitProgramArguments(ProgramArguments &node) {
    if (node.literals) node.literals->accept(*this);
}

void SymbolTableVisitor::visitLiterals(Literals &node) {
    for (const auto &literal: node.literals) {
        literal->accept(*this);
    }
}

void SymbolTableVisitor::visitLiteral(Literal &node) {
}

void SymbolTableVisitor::visitArguments(Arguments &node) {
    if (node.expressions) node.expressions->accept(*this);
}

void SymbolTableVisitor::visitExpressions(Expressions &node) {
    for (const auto &expression: node.expressions) {
        expression->accept(*this);
    }
}

void SymbolTableVisitor::visitExpression(Expression &node) {
    node.accept(*this);
}

void SymbolTableVisitor::visitPrimary(Primary &node) {
    if (node.class_name) node.class_name->accept(*this);
}

void SymbolTableVisitor::visitCompoundExpression(CompoundExpression &node) {
    // TODO check if identifier is a variable or a class or a method

    if (symbolTable.getIdentifierType(node.identifier) == ID_VARIABLE) {
        std::string variableType = symbolTable.lookupVariable(node.identifier, node.span)->type;
        symbolTable.makeVariableUsed(node.identifier);
        // Here we know the type of identifier. Next expression must be a function call -> we can check if the method is defined for the variable type
        if (node.compoundExpressions.empty()) {

        } else {
            std::string methodName = node.compoundExpressions[0]->identifier;
            std::vector<std::string> argTypes;
            if (node.compoundExpressions[0]->arguments) {
                for (const auto &arg: node.compoundExpressions[0]->arguments->expressions->expressions) {
                    arg->accept(*this);
                    argTypes.push_back(arg->get_type(symbolTable.identifierStringTypes));
                }
            }

            symbolTable.lookupFunction(variableType, methodName, argTypes, node.span);
        }

    } else if (symbolTable.getIdentifierType(node.identifier) == ID_CLASS) {
        //llvmSymbolTable.lookupClass(node.identifier, node.span);
    } else if (symbolTable.getIdentifierType(node.identifier) == ID_FUNCTION) {
        //llvmSymbolTable.lookupFunction(node.identifier, node.span);
    } else {
        throw std::runtime_error("Identifier " + node.identifier + " is not declared");
    }


    if (node.arguments) node.arguments->accept(*this);
    for (const auto &compoundExpression: node.compoundExpressions) {
        compoundExpression->accept(*this);
    }
}

void SymbolTableVisitor::visitClassDeclarations(ClassDeclarations &node) {
    for (const auto &classDeclaration: node.classDeclarations) {
        symbolTable.addClassEntry(classDeclaration->className->name, classDeclaration->className->span);
        classDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitClassDeclaration(ClassDeclaration &node) {
    symbolTable.enterScope();
    symbolTable.currClassName = node.className->name;
    if (node.className) node.className->accept(*this);
    if (node.extension) node.extension->accept(*this);
    if (node.classBody) node.classBody->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitClassBody(ClassBody &node) {
    if (node.memberDeclarations) node.memberDeclarations->accept(*this);
}

void SymbolTableVisitor::visitExtension(Extension &node) {
    if (node.className) node.className->accept(*this);
}

void SymbolTableVisitor::visitBody(Body &node) {
    if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
}

void SymbolTableVisitor::visitBodyDeclarations(BodyDeclarations &node) {
    for (auto &bodyDeclaration: node.bodyDeclarations) {
        if (bodyDeclaration) bodyDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitBodyDeclaration(BodyDeclaration &node) {
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    if (node.statement) node.statement->accept(*this);
}

void SymbolTableVisitor::visitStatement(Statement &node) {
    node.accept(*this);
}

void SymbolTableVisitor::visitIfStatement(IfStatement &node) {
    if (node.expression) node.expression->accept(*this);
    if (node.ifBranch) node.ifBranch->accept(*this);
    if (node.elseBranch) node.elseBranch->accept(*this);
}

void SymbolTableVisitor::visitIfBranch(IfBranch &node) {
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitElseBranch(ElseBranch &node) {
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitWhileLoop(WhileLoop &node) {
    symbolTable.enterScope();
    if (node.body) node.body->accept(*this);
    if (node.expression) node.expression->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitAssignment(Assignment &node) {
    // [CHECK] if variable is declared
    try {
        symbolTable.lookupVariable(node.variableName->name, node.variableName->span);
        symbolTable.makeVariableUsed(node.variableName->name);
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }

    const auto span = node.variableName->span;

    // [CHECK] if variable is assigned to the correct statementType
    const auto variableType = symbolTable.lookupVariable(node.variableName->name, span)->type;

    std::string expressionType;

    if (node.expression->isCompound) {
        CompoundExpression *expression = dynamic_cast<CompoundExpression *>(node.expression.get());
        // auto compoundExpression = std::move(node.expression->compoundExpression);
        // std::string compoundClass = compoundExpression->identifier;
        // while (true)
        // {
        //     std::cout << compoundExpression->compoundExpressions.size() << std::endl;
        //     if (compoundExpression->compoundExpressions.size() > 1)
        //         throw std::runtime_error("AAAAA");
        //     if (compoundExpression->compoundExpressions.empty())
        //         break;
        //     if (compoundExpression->compoundExpressions.front() == nullptr)
        //         break;
        //     compoundExpression = std::move(compoundExpression->compoundExpressions.front());
        // }

        // const auto expressionClass = llvmSymbolTable.lookupClass(compoundExpression->identifier,
        //                                                      compoundExpression->span, false);
        // if (expressionClass == nullptr)
        // {
        //     // compoundExpression->arguments->expressions->expressions
        //     // const auto expressionFunction = llvmSymbolTable.lookupFunction(compoundExpression->identifier,
        //     //                                                            compoundExpression->span, false);
        //     const MethodEntry* expressionFunction = nullptr;
        //     if (expressionFunction == nullptr)
        //     {
        //         const auto expressionVariable = llvmSymbolTable.lookupVariable(compoundExpression->identifier,
        //                                                                    compoundExpression->span, false);
        //         if (expressionVariable == nullptr)
        //         {
        //             expressionType = variableType;
        //             // throw std::runtime_error(
        //             //     "Variable " + node.variableName->name + " not found" +
        //             //     " at line: " + std::to_string(span.get_line_num()) +
        //             //     " column: " + std::to_string(span.get_pos_begin())
        //             // );
        //         }
        //         else
        //         {
        //             expressionType = expressionVariable->statementType;
        //         }
        //     }
        //     else
        //     {
        //         expressionType = expressionFunction->returnType;
        //     }
        // }
        // else
        // {
        //     expressionType = expressionClass->name;
        // }
        expressionType = variableType;
    } else {
        Primary *expression = dynamic_cast<Primary *>(node.expression.get());
        if (expression->literal) {
            switch (expression->literal->type) {
                case BOOL_LITERAL:
                    expressionType = "Boolean";
                    break;
                case INT_LITERAL:
                    expressionType = "Integer";
                    break;
                case REAL_LITERAL:
                    expressionType = "Real";
                    break;
                default:
                    expressionType = variableType;
                    break;
            }
        } else if (expression->class_name) {
            expressionType = expression->class_name->name;
        }
    }


    if (variableType != expressionType) {
        throw std::runtime_error(
                "Variable " + node.variableName->name + " is of statementType " + variableType +
                " but is being assigned to statementType " + expressionType +
                " at line: " + std::to_string(span.get_line_num()) +
                " column: " + std::to_string(span.get_pos_begin())
        );
    }

    if (node.expression) node.expression->accept(*this);
    if (node.variableName) node.variableName->accept(*this);
}

void SymbolTableVisitor::visitMemberDeclarations(MemberDeclarations &node) {
    for (const auto &memberDeclaration: node.member_declarations) {
        memberDeclaration->accept(*this);
    }
}

void SymbolTableVisitor::visitMemberDeclaration(MemberDeclaration &node) {
    if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
    if (node.methodDeclaration) node.methodDeclaration->accept(*this);
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
}

void SymbolTableVisitor::visitConstructorDeclaration(ConstructorDeclaration &node) {
    symbolTable.enterScope();
    if (node.parameters) node.parameters->accept(*this);
    auto paramNames = std::vector<std::string>();

    // Add parameters to the symbol table
    if (node.parameters) {
        for (const auto &parameter: node.parameters->parameters) {
            paramNames.push_back(parameter->className->name);
            symbolTable.addVariableEntry(parameter->name, parameter->className->name, parameter->span);
        }
    }

    symbolTable.addFunctionEntry("Create_Default", node.className, "void", node.span, paramNames);
    if (node.body) node.body->accept(*this);

    const ReturnStatement *returnStatement = nullptr;
    for (const auto &bodyDeclaration: node.body->bodyDeclarations->bodyDeclarations) {
        if (bodyDeclaration && bodyDeclaration->statement && bodyDeclaration->statement->statementType ==
                                                             RETURN_STATEMENT) {
            // dynamic cast statement to ReturnStatement
            returnStatement = dynamic_cast<ReturnStatement *>(bodyDeclaration->statement.get());
        }
    }


    // [CHECK] if constructor has return statement
    if (returnStatement != nullptr) {
        throw std::runtime_error("Constructor cannot have return statement");
    }

    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitReturnStatement(ReturnStatement &node) {
    if (node.expression) node.expression->accept(*this);
}

void SymbolTableVisitor::visitVariableDeclaration(VariableDeclaration &node) {
    if (node.expression->isCompound) {
        CompoundExpression *expression = dynamic_cast<CompoundExpression *>(node.expression.get());
        if (expression->compoundExpressions.empty()) {
            symbolTable.lookupClass(expression->identifier,
                                    expression->span);
        }
        symbolTable.addVariableEntry(node.variable->name, expression->identifier,
                                     node.variable->span);
    } else {
        Primary *expression = dynamic_cast<Primary *>(node.expression.get());
        if (expression->literal->type == LiteralType::BOOL_LITERAL) {
            symbolTable.addVariableEntry(node.variable->name, "Boolean", node.variable->span);
        } else if (expression->literal->type == LiteralType::INT_LITERAL) {
            symbolTable.addVariableEntry(node.variable->name, "Integer", node.variable->span);
        } else if (expression->literal->type == LiteralType::REAL_LITERAL) {
            symbolTable.addVariableEntry(node.variable->name, "Real", node.variable->span);
        }
    }


    if (node.expression) node.expression->accept(*this);
    if (node.variable) node.variable->accept(*this);
}

void SymbolTableVisitor::visitMethodDeclaration(MethodDeclaration &node) {
    symbolTable.enterScope();
    auto paramNames = std::vector<std::string>();

    if (node.parameters) {
        for (const auto &parameter: node.parameters->parameters) {
            paramNames.push_back(parameter->className->name);
            symbolTable.addVariableEntry(parameter->name, parameter->className->name, parameter->span);
        }
    }

    symbolTable.addFunctionEntry(
            node.methodName->name,
            symbolTable.currClassName,
            node.returnType ? node.returnType->className->name : "void",
            node.methodName->span,
            paramNames
    );

    if (node.body) node.body->accept(*this);
    if (node.parameters) node.parameters->accept(*this);

    // [CHECK] if return statementType is correct
    const auto expectedReturnType = symbolTable.lookupFunction(
            symbolTable.currClassName, node.methodName->name, paramNames, node.methodName->span
    )->returnType;
    const ReturnStatement *returnStatement = nullptr;
    for (const auto &bodyDeclaration: node.body->bodyDeclarations->bodyDeclarations) {
        if (bodyDeclaration && bodyDeclaration->statement &&
            bodyDeclaration->statement->statementType == RETURN_STATEMENT) {
            returnStatement = dynamic_cast<ReturnStatement *>(bodyDeclaration->statement.get());
        }
    }


    if (returnStatement == nullptr) {
        if (expectedReturnType != "void") {
            throw std::runtime_error(
                    "Method " + node.methodName->name + " is of statementType " + expectedReturnType +
                    " but is being assigned to statementType void");
        }
    } else {
        // TODO check statementType of the last element in the compound expression
        if (returnStatement->expression->isCompound) {
            CompoundExpression *expression = dynamic_cast<CompoundExpression *>(returnStatement->expression.get());
            const auto span = returnStatement->expression->span;
            auto type = expression->get_type(symbolTable.identifierStringTypes);
            if (type != expectedReturnType) {
                throw std::runtime_error(
                        "Method " + node.methodName->name + " is of statementType " + expectedReturnType +
                        " but is being assigned to statementType " + type +
                        " at line: " + std::to_string(span.get_line_num()) +
                        " column: " + std::to_string(span.get_pos_begin())
                );
            }


        }

        // TODO check for primary expression


    }

    if (node.returnType) node.returnType->accept(*this);
    if (node.methodName) node.methodName->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitMethodName(MethodName &node) {
}

void SymbolTableVisitor::visitParameters(Parameters &node) {
    for (const auto &parameter: node.parameters) {
        parameter->accept(*this);
    }
}

void SymbolTableVisitor::visitParameter(Parameter &node) {
    if (node.className) node.className->accept(*this);
}

void SymbolTableVisitor::visitReturnType(ReturnType &node) {
}

void SymbolTableVisitor::visitVariableName(VariableName &node) {
}
