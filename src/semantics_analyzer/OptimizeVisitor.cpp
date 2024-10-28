#include <iostream>
#include <stdexcept>
#include "OptimizeVisitor.h"


OptimizeVisitor::~OptimizeVisitor() = default;

void OptimizeVisitor::visitProgram(const Program& node)
{
    if (node.programDeclaration) node.programDeclaration->accept(*this);
    if (node.classDeclarations) node.classDeclarations->accept(*this);
}

void OptimizeVisitor::visitProgramDeclaration(const ProgramDeclaration& node)
{
    // TODO handle classes
    // symbolTable.addClassEntry(node.className->name, "ProgramType");
    if (node.className) node.className->accept(*this);
    if (node.arguments) node.arguments->accept(*this);
}

void OptimizeVisitor::visitClassName(const ClassName& node)
{
}

void OptimizeVisitor::visitProgramArguments(const ProgramArguments& node)
{
    if (node.literals) node.literals->accept(*this);
}

void OptimizeVisitor::visitLiterals(const Literals& node)
{
    for (auto& literal : node.literals)
    {
        literal->accept(*this);
    }
}

void OptimizeVisitor::visitLiteral(const Literal& node)
{
}

void OptimizeVisitor::visitArguments(const Arguments& node)
{
    if (node.expressions) node.expressions->accept(*this);
}

void OptimizeVisitor::visitExpressions(const Expressions& node)
{
    for (auto& expression : node.expressions)
    {
        expression->accept(*this);
    }
}

void OptimizeVisitor::visitExpression(const Expression& node)
{
    if (node.primary) node.primary->accept(*this);
    if (node.compoundExpression) node.compoundExpression->accept(*this);
}

void OptimizeVisitor::visitPrimary(const Primary& node)
{
    if (node.class_name) node.class_name->accept(*this);
}

void OptimizeVisitor::visitCompoundExpression(const CompoundExpression& node)
{
    if (node.arguments) node.arguments->accept(*this);
    for (auto& compoundExpression : node.compoundExpressions)
    {
        compoundExpression->accept(*this);
    }
}

void OptimizeVisitor::visitClassDeclarations(const ClassDeclarations& node)
{
    for (auto& classDeclaration : node.classDeclarations)
    {
        classDeclaration->accept(*this);
    }
}

void OptimizeVisitor::visitClassDeclaration(const ClassDeclaration& node)
{
    if (node.className) node.className->accept(*this);
    if (node.extension) node.extension->accept(*this);
    if (node.classBody) node.classBody->accept(*this);
}

void OptimizeVisitor::visitClassBody(const ClassBody& node)
{
    if (node.memberDeclarations) node.memberDeclarations->accept(*this);
}

void OptimizeVisitor::visitExtension(const Extension& node)
{
    if (node.className) node.className->accept(*this);
}

void OptimizeVisitor::visitBody(const Body& node)
{
    if (node.bodyDeclarations) node.bodyDeclarations->accept(*this);
}

void OptimizeVisitor::visitBodyDeclarations(const BodyDeclarations& node)
{
    for (auto& bodyDeclaration : node.bodyDeclarations)
    {
        if (bodyDeclaration) bodyDeclaration->accept(*this);
    }
}

void OptimizeVisitor::visitBodyDeclaration(const BodyDeclaration& node)
{
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
    if (node.statement) node.statement->accept(*this);
}

void OptimizeVisitor::visitStatement(const Statement& node)
{
   node.accept(*this);
}

void OptimizeVisitor::visitIfStatement(const IfStatement& node)
{
    if(node.expression && node.expression->primary && node.expression->primary->literal)
    {
        auto literal = std::move(node.expression->primary->literal);
        if(literal->type==LiteralType::BOOL_LITERAL)
        {
            BoolLiteral* boolLiteral = dynamic_cast<BoolLiteral*>(literal.get());
            BodyDeclaration* bodyDeclaration = node.parent;
            BodyDeclarations* parentBodyDeclarations = bodyDeclaration->parent;
             int indexOfCurrentBodyDeclaration = -1;
            for(int i = 0; i<parentBodyDeclarations->bodyDeclarations.size(); i++)
            {
                if(parentBodyDeclarations->bodyDeclarations[i].get() == bodyDeclaration)
                {
                    indexOfCurrentBodyDeclaration = i;
                    break;
                }
            }
            if(boolLiteral->value== true)
            {

                auto bodyDeclarations = std::move(node.ifBranch->body->bodyDeclarations);
               // parentBodyDeclarations->bodyDeclarations

                //
                // Remove bodyDeclaration with index indexOfCurrentBodyDeclaration
                // Add bodyDeclarations to parentBodyDeclarations at index indexOfCurrentBodyDeclaration

                for(int i = 0; i<bodyDeclarations->bodyDeclarations.size(); i++)
                {
                    parentBodyDeclarations->bodyDeclarations.insert(parentBodyDeclarations->bodyDeclarations.begin()+indexOfCurrentBodyDeclaration+i, std::move(bodyDeclarations->bodyDeclarations[i]));
                }

                parentBodyDeclarations->bodyDeclarations.erase(parentBodyDeclarations->bodyDeclarations.begin()+indexOfCurrentBodyDeclaration+bodyDeclarations->bodyDeclarations.size());
                bodyDeclaration->statement = nullptr;

            }
            else
            {

                // Remove if branch
                auto bodyDeclarations = std::move(node.elseBranch->body->bodyDeclarations);
                // parentBodyDeclarations->bodyDeclarations

                //
                // Remove bodyDeclaration with index indexOfCurrentBodyDeclaration
                // Add bodyDeclarations to parentBodyDeclarations at index indexOfCurrentBodyDeclaration

                for(int i = 0; i<bodyDeclarations->bodyDeclarations.size(); i++)
                {
                    parentBodyDeclarations->bodyDeclarations.insert(parentBodyDeclarations->bodyDeclarations.begin()+indexOfCurrentBodyDeclaration+i, std::move(bodyDeclarations->bodyDeclarations[i]));
                }

                parentBodyDeclarations->bodyDeclarations.erase(parentBodyDeclarations->bodyDeclarations.begin()+indexOfCurrentBodyDeclaration+bodyDeclarations->bodyDeclarations.size());
                bodyDeclaration->statement = nullptr;
            }
        }
        // Check if literal type is BoolLiteral


    }
    if (node.expression) node.expression->accept(*this);
    if (node.ifBranch) node.ifBranch->accept(*this);
    if (node.elseBranch) node.elseBranch->accept(*this);
}

void OptimizeVisitor::visitIfBranch(const IfBranch& node)
{
    if (node.body) node.body->accept(*this);
}

void OptimizeVisitor::visitElseBranch(const ElseBranch& node)
{
    if (node.body) node.body->accept(*this);
}

void OptimizeVisitor::visitWhileLoop(const WhileLoop& node)
{
    if (node.body) node.body->accept(*this);
    if (node.expression) node.expression->accept(*this);
}

void OptimizeVisitor::visitAssignment(const Assignment& node)
{
    if (node.expression) node.expression->accept(*this);
    if (node.variableName) node.variableName->accept(*this);
}

void OptimizeVisitor::visitMemberDeclarations(const MemberDeclarations& node)
{
    for (auto& memberDeclaration : node.member_declarations)
    {
        memberDeclaration->accept(*this);
    }
}

void OptimizeVisitor::visitMemberDeclaration(const MemberDeclaration& node)
{
    if (node.constructorDeclaration) node.constructorDeclaration->accept(*this);
    if (node.methodDeclaration) node.methodDeclaration->accept(*this);
    if (node.variableDeclaration) node.variableDeclaration->accept(*this);
}

void OptimizeVisitor::visitConstructorDeclaration(const ConstructorDeclaration& node)
{
    if (node.parameters) node.parameters->accept(*this);

    if (node.body) node.body->accept(*this);
}

void OptimizeVisitor::visitReturnStatement(const ReturnStatement& node)
{
    if (node.expression) node.expression->accept(*this);
}

void OptimizeVisitor::visitVariableDeclaration(const VariableDeclaration& node)
{
    if (symbolTable.unusedVariables.find(node.variable->name) != symbolTable.unusedVariables.end()){
        // TODO remove variable from ast
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

void OptimizeVisitor::visitMethodDeclaration(const MethodDeclaration& node)
{

    if (node.body) node.body->accept(*this);
    if (node.parameters) node.parameters->accept(*this);

    if (node.returnType) node.returnType->accept(*this);
    if (node.methodName) node.methodName->accept(*this);

}

void OptimizeVisitor::visitMethodName(const MethodName& node)
{
}

void OptimizeVisitor::visitParameters(const Parameters& node)
{
    for (auto& parameter : node.parameters)
    {
        parameter->accept(*this);
    }
}

void OptimizeVisitor::visitParameter(const Parameter& node)
{
    if (node.className) node.className->accept(*this);
}

void OptimizeVisitor::visitReturnType(const ReturnType& node)
{
}

void OptimizeVisitor::visitVariableName(const VariableName& node)
{
}
