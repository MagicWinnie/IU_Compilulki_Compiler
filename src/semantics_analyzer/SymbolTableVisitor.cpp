#include <iostream>
#include <stdexcept>
#include "SymbolTableVisitor.h"

SymbolTableVisitor::SymbolTableVisitor() {
    symbolTable.enterScope();
    symbolTable.addClassEntry("Integer", Span(0, 0, 0));
    symbolTable.addClassEntry("Boolean", Span(0, 0, 0));
    symbolTable.addClassEntry("Real", Span(0, 0, 0));
    symbolTable.addClassEntry("IntArray", Span(0, 0, 0));
    symbolTable.addClassEntry("RealArray", Span(0, 0, 0));
    symbolTable.addClassEntry("IntList", Span(0, 0, 0));


    // Integer
    symbolTable.addFunctionEntry("Create_Default", "Integer", "void", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("Create_Default", "Integer", "void", Span(0, 0, 0), {"Integer"});
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
    symbolTable.addFunctionEntry("scan", "Integer", "void", Span(0, 0, 0), {});

    // Real
    symbolTable.addFunctionEntry("Create_Default", "Real", "void", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("Create_Default", "Real", "void", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("toInteger", "Real", "Integer", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("UnaryMinus", "Real", "Real", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("Plus", "Real", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Plus", "Real", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Minus", "Real", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Minus", "Real", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Mult", "Real", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Mult", "Real", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Div", "Real", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Div", "Real", "Real", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Rem", "Real", "Real", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Less", "Real", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Less", "Real", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("LessEqual", "Real", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("LessEqual", "Real", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Greater", "Real", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Greater", "Real", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("GreaterEqual", "Real", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("GreaterEqual", "Real", "Boolean", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Equal", "Real", "Boolean", Span(0, 0, 0), {"Real"});
    symbolTable.addFunctionEntry("Equal", "Real", "Boolean", Span(0, 0, 0), {"Integer"});

    symbolTable.addFunctionEntry("print", "Real", "void", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("scan", "Real", "String", Span(0, 0, 0), {});

    // Boolean
    symbolTable.addFunctionEntry("Create_Default", "Boolean", "void", Span(0, 0, 0), {""});
    symbolTable.addFunctionEntry("Create_Default", "Boolean", "void", Span(0, 0, 0), {"Boolean"});
    symbolTable.addFunctionEntry("Or", "Boolean", "Boolean", Span(0, 0, 0), {"Boolean"});
    symbolTable.addFunctionEntry("And", "Boolean", "Boolean", Span(0, 0, 0), {"Boolean"});
    symbolTable.addFunctionEntry("Xor", "Boolean", "Boolean", Span(0, 0, 0), {"Boolean"});
    symbolTable.addFunctionEntry("Not", "Boolean", "Boolean", Span(0, 0, 0), {});

    symbolTable.addFunctionEntry("print", "Boolean", "void", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("scan", "Boolean", "String", Span(0, 0, 0), {});

    //IntArray
    symbolTable.addFunctionEntry("Create_Default", "IntArray", "void", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("Length", "IntArray", "Integer", Span(0, 0, 0), {});
    symbolTable.addFunctionEntry("get", "IntArray", "Integer", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("set", "IntArray", "void", Span(0, 0, 0), {"Integer", "Integer"});
    symbolTable.addFunctionEntry("print", "IntArray", "void", Span(0, 0, 0), {});

    //IntList
    symbolTable.addFunctionEntry("append", "IntList", "void", Span(0, 0, 0), {"Integer"});
    symbolTable.addFunctionEntry("head", "IntList", "Integer", Span(0, 0, 0), {});
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

void SymbolTableVisitor::compoundExpressionParser(const CompoundExpression &node, const bool isFirst,
                                                  std::string previousType) {
    std::vector<std::string> argTypes;
    if (node.arguments) {
        for (const auto &arg: node.arguments->expressions->expressions) {
            if (arg->isCompound) {
                const auto compoundArgument = dynamic_cast<CompoundExpression *>(arg.get());
                compoundExpressionParser(*compoundArgument, true, symbolTable.currClassName);
            }
            argTypes.push_back(arg->get_type(symbolTable));
        }
    }

    const auto identifierType = symbolTable.getIdentifierType(node.identifier);
    if (identifierType == ID_VARIABLE) {
        previousType = symbolTable.lookupVariable(node.identifier, node.span)->type;
        symbolTable.makeVariableUsed(node.identifier);
    } else if (identifierType == ID_CLASS) {
        previousType = node.identifier;
        if (node.arguments) {
            symbolTable.lookupFunction(previousType, "Create_Default", argTypes, node.span);
        }
        //llvmSymbolTable.lookupClass(node.identifier, node.span);
    } else if (identifierType == ID_FUNCTION) {
        if (isFirst) {
            throw std::runtime_error(
                "Variable " + node.identifier + " is not declared " +
                "(if you are trying to call local method use this." + node.identifier + ")"
                " at line: " + std::to_string(node.span.get_line_num()) +
                " column: " + std::to_string(node.span.get_pos_begin())
            );
        }

        previousType = symbolTable.lookupFunction(
            previousType, node.identifier, argTypes, node.span
        )->returnType;

        //llvmSymbolTable.lookupFunction(node.identifier, node.span);
    } else {
        throw std::runtime_error(
            "Identifier " + node.identifier + " is not declared" +
            " at line: " + std::to_string(node.span.get_line_num()) +
            " column: " + std::to_string(node.span.get_pos_begin())
        );
    }

    for (const auto &compoundExpression: node.compoundExpressions) {
        compoundExpressionParser(*compoundExpression, false, previousType);
    }
}

void SymbolTableVisitor::visitCompoundExpression(CompoundExpression &node) {
    compoundExpressionParser(node, true, symbolTable.currClassName);
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
    symbolTable.addVariableEntry("this", node.className->name, node.className->span);
    if (node.className) node.className->accept(*this);
    if (node.extension) node.extension->accept(*this);
    if (node.classBody) node.classBody->accept(*this);
    symbolTable.leaveScope();
}

void SymbolTableVisitor::visitClassBody(ClassBody &node) {
    if (node.memberDeclarations) node.memberDeclarations->accept(*this);
}

void SymbolTableVisitor::visitExtension(Extension &node) {
    if (node.className) {
        symbolTable.lookupClass(symbolTable.currClassName)->setParentClass(symbolTable.lookupClass(node.className->name));
        node.className->accept(*this);
    }
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
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }

    const auto span = node.variableName->span;

    // [CHECK] if variable is assigned to the correct statementType
    const auto variableType = symbolTable.lookupVariable(node.variableName->name, span)->type;

    const std::string expressionType = node.expression->get_type(symbolTable);

    if (variableType != expressionType) {
        throw std::runtime_error(
            "Variable " + node.variableName->name + " is of type " + variableType +
            " but is being assigned to type " + expressionType +
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
        if (bodyDeclaration &&
            bodyDeclaration->statement &&
            bodyDeclaration->statement->statementType == RETURN_STATEMENT) {
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
        auto *expression = dynamic_cast<CompoundExpression *>(node.expression.get());
        if (expression->compoundExpressions.empty()) {
            symbolTable.lookupClass(expression->identifier,
                                    expression->span);
        }
        symbolTable.addVariableEntry(node.variable->name, expression->get_type(symbolTable), node.variable->span);
    } else {
        const auto *expression = dynamic_cast<Primary *>(node.expression.get());
        if (expression->literal->type == BOOL_LITERAL) {
            symbolTable.addVariableEntry(node.variable->name, "Boolean", node.variable->span);
        } else if (expression->literal->type == INT_LITERAL) {
            symbolTable.addVariableEntry(node.variable->name, "Integer", node.variable->span);
        } else if (expression->literal->type == REAL_LITERAL) {
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
                "Method " + node.methodName->name + " is of type " + expectedReturnType +
                " but is being assigned to type void");
        }
    } else {
        // TODO check statementType of the last element in the compound expression
        if (returnStatement->expression->isCompound) {
            auto *expression = dynamic_cast<CompoundExpression *>(returnStatement->expression.get());
            const auto span = returnStatement->expression->span;
            const auto type = expression->get_type(symbolTable);
            if (type != expectedReturnType) {
                throw std::runtime_error(
                    "Method " + node.methodName->name + " is of type " + expectedReturnType +
                    " but is being assigned to type " + type +
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
