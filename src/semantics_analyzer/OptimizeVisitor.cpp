#include <iostream>
#include "OptimizeVisitor.h"


OptimizeVisitor::~OptimizeVisitor() = default;

void OptimizeVisitor::visitProgram(Program& node)
{
    if (node.programDeclaration) node.programDeclaration->accept(*this);
    if (node.classDeclarations) node.classDeclarations->accept(*this);
}

void OptimizeVisitor::visitProgramDeclaration(ProgramDeclaration& node)
{
    if (node.className) node.className->accept(*this);
    if (node.arguments) node.arguments->accept(*this);
}

void OptimizeVisitor::visitClassName(ClassName& node)
{
}

void OptimizeVisitor::visitProgramArguments(ProgramArguments& node)
{
    if (node.literals) node.literals->accept(*this);
}

void OptimizeVisitor::visitLiterals(Literals& node)
{
    for (const auto& literal : node.literals)
    {
        literal->accept(*this);
    }
}

void OptimizeVisitor::visitLiteral(Literal& node)
{
}

void OptimizeVisitor::visitArguments(Arguments& node)
{
    if (node.expressions) node.expressions->accept(*this);
}

void OptimizeVisitor::visitExpressions(Expressions& node)
{
    for (const auto& expression : node.expressions)
    {
        expression->accept(*this);
    }
}

void OptimizeVisitor::visitExpression(Expression& node)
{
    if (node.primary) node.primary->accept(*this);
    if (node.compoundExpression) node.compoundExpression->accept(*this);
}

void OptimizeVisitor::visitPrimary(Primary& node)
{
    if (node.class_name) node.class_name->accept(*this);
}

void OptimizeVisitor::visitCompoundExpression(CompoundExpression& node)
{
    if (node.arguments) node.arguments->accept(*this);
    for (const auto& compoundExpression : node.compoundExpressions)
    {
        compoundExpression->accept(*this);
    }
}

void OptimizeVisitor::visitClassDeclarations(ClassDeclarations& node)
{
    for (const auto& classDeclaration : node.classDeclarations)
    {
        classDeclaration->accept(*this);
    }
}

void OptimizeVisitor::visitClassDeclaration(ClassDeclaration& node)
{
    if (node.className) node.className->accept(*this);
    if (node.extension) node.extension->accept(*this);
    if (node.classBody) node.classBody->accept(*this);
}

void OptimizeVisitor::visitClassBody(ClassBody& node)
{
    if (node.memberDeclarations) node.memberDeclarations->accept(*this);
}

void OptimizeVisitor::visitExtension(Extension& node)
{
    if (node.className) node.className->accept(*this);
}

void OptimizeVisitor::visitBody(Body& node)
{
    if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
}

void OptimizeVisitor::visitBodyDeclarations(BodyDeclarations& node)
{
    for (int i = 0; i < node.bodyDeclarations.size(); i++)
    {
        auto& bodyDeclaration = node.bodyDeclarations[i];
        if (bodyDeclaration)
        {
            bodyDeclaration->accept(*this);
            if (bodyDeclaration->statement && bodyDeclaration->statement->type == RETURN_STATEMENT)
            {
                node.bodyDeclarations.erase(node.bodyDeclarations.begin() + i + 1, node.bodyDeclarations.end());
                break;
            }
        }
    }
}

void OptimizeVisitor::visitBodyDeclaration(BodyDeclaration& node)
{
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    if (node.statement) node.statement->accept(*this);
}

void OptimizeVisitor::visitStatement(Statement& node)
{
    node.accept(*this);
}

void OptimizeVisitor::visitIfStatement(IfStatement& node)
{
    if (node.expression && node.expression->primary && node.expression->primary->literal)
    {
        const auto literal = std::move(node.expression->primary->literal);
        if (literal->type == BOOL_LITERAL)
        {
            const auto* boolLiteral = dynamic_cast<BoolLiteral*>(literal.get());
            BodyDeclaration* bodyDeclaration = node.parent;
            BodyDeclarations* parentBodyDeclarations = bodyDeclaration->parent;
            int indexOfCurrentBodyDeclaration = -1;
            for (int i = 0; i < parentBodyDeclarations->bodyDeclarations.size(); i++)
            {
                if (parentBodyDeclarations->bodyDeclarations[i].get() == bodyDeclaration)
                {
                    indexOfCurrentBodyDeclaration = i;
                    break;
                }
            }
            if (boolLiteral->value == true)
            {
                const auto bodyDeclarations = std::move(node.ifBranch->body->bodyDeclarations);
                // parentBodyDeclarations->bodyDeclarations

                //
                // Remove bodyDeclaration with index indexOfCurrentBodyDeclaration
                // Add bodyDeclarations to parentBodyDeclarations at index indexOfCurrentBodyDeclaration

                for (int i = 0; i < bodyDeclarations->bodyDeclarations.size(); i++)
                {
                    parentBodyDeclarations->bodyDeclarations.insert(
                        parentBodyDeclarations->bodyDeclarations.begin() + indexOfCurrentBodyDeclaration + i,
                        std::move(bodyDeclarations->bodyDeclarations[i]));
                }

                parentBodyDeclarations->bodyDeclarations.erase(
                    parentBodyDeclarations->bodyDeclarations.begin() +
                    indexOfCurrentBodyDeclaration +
                    bodyDeclarations->bodyDeclarations.size()
                );
                bodyDeclaration->statement = nullptr;
            }
            else
            {
                // Remove if branch
                const auto bodyDeclarations = std::move(node.elseBranch->body->bodyDeclarations);
                // parentBodyDeclarations->bodyDeclarations

                //
                // Remove bodyDeclaration with index indexOfCurrentBodyDeclaration
                // Add bodyDeclarations to parentBodyDeclarations at index indexOfCurrentBodyDeclaration

                for (int i = 0; i < bodyDeclarations->bodyDeclarations.size(); i++)
                {
                    parentBodyDeclarations->bodyDeclarations.insert(
                        parentBodyDeclarations->bodyDeclarations.begin() + indexOfCurrentBodyDeclaration + i,
                        std::move(bodyDeclarations->bodyDeclarations[i]));
                }

                parentBodyDeclarations->bodyDeclarations.erase(
                    parentBodyDeclarations->bodyDeclarations.begin() +
                    indexOfCurrentBodyDeclaration +
                    bodyDeclarations->bodyDeclarations.size()
                );
                bodyDeclaration->statement = nullptr;
            }
        }
        // Check if literal type is BoolLiteral
    }
    if (node.expression) node.expression->accept(*this);
    if (node.ifBranch) node.ifBranch->accept(*this);
    if (node.elseBranch) node.elseBranch->accept(*this);
}

void OptimizeVisitor::visitIfBranch(IfBranch& node)
{
    if (node.body) node.body->accept(*this);
}

void OptimizeVisitor::visitElseBranch(ElseBranch& node)
{
    if (node.body) node.body->accept(*this);
}

void OptimizeVisitor::visitWhileLoop(WhileLoop& node)
{
    if (node.body) node.body->accept(*this);
    if (node.expression) node.expression->accept(*this);
}

void OptimizeVisitor::visitAssignment(Assignment& node)
{
    if (node.expression) node.expression->accept(*this);
    if (node.variableName) node.variableName->accept(*this);
}

void OptimizeVisitor::visitMemberDeclarations(MemberDeclarations& node)
{
    for (const auto& memberDeclaration : node.member_declarations)
    {
        memberDeclaration->accept(*this);
    }
}

void OptimizeVisitor::visitMemberDeclaration(MemberDeclaration& node)
{
    if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
    if (node.methodDeclaration) node.methodDeclaration->accept(*this);
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
}

void OptimizeVisitor::visitConstructorDeclaration(ConstructorDeclaration& node)
{
    if (node.parameters) node.parameters->accept(*this);

    if (node.body) node.body->accept(*this);
}

void OptimizeVisitor::visitReturnStatement(ReturnStatement& node)
{
    if (node.expression) node.expression->accept(*this);
}

void OptimizeVisitor::visitVariableDeclaration(VariableDeclaration& node)
{
    if (symbolTable.unusedVariables.find(node.variable->name) != symbolTable.unusedVariables.end())
    {
        std::cout << "Unused variable: " << node.variable->name << std::endl;
        if (node.bodyParent)
        {
            node.bodyParent->variableDeclaration = nullptr;
        }
        if (node.memberParent)
        {
            node.memberParent->variableDeclaration = nullptr;
        }
    }

    if (node.expression) node.expression->accept(*this);
    if (node.variable) node.variable->accept(*this);
}

void OptimizeVisitor::visitMethodDeclaration(MethodDeclaration& node)
{
    if (node.body) node.body->accept(*this);
    if (node.parameters) node.parameters->accept(*this);

    if (node.returnType) node.returnType->accept(*this);
    if (node.methodName) node.methodName->accept(*this);
}

void OptimizeVisitor::visitMethodName(MethodName& node)
{
}

void OptimizeVisitor::visitParameters(Parameters& node)
{
    for (const auto& parameter : node.parameters)
    {
        parameter->accept(*this);
    }
}

void OptimizeVisitor::visitParameter(Parameter& node)
{
    if (node.className) node.className->accept(*this);
}

void OptimizeVisitor::visitReturnType(ReturnType& node)
{
}

void OptimizeVisitor::visitVariableName(VariableName& node)
{
}
