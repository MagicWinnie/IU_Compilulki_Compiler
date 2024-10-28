#include <iostream>

#include "lexical_analyzer/lexer.h"
#include "semantics_analyzer/OptimizeVisitor.h"
#include "syntax_analyzer/Parser.h"
#include "settings/settings.h"
#include "semantics_analyzer/SymbolTableVisitor.h"


int main(const int argc, char* argv[])
{
    auto settings = Settings();
    settings.parse(argc, argv);
    settings.process();

    if (settings.get_debug())
    {
        std::cout << settings << std::endl;
    }

    auto lexer = Lexer(settings.get_infile(), settings.get_debug());
    auto tokens = lexer.parse();

    auto parser = Parser(std::move(tokens), settings.get_infile(), settings.get_debug());
    std::unique_ptr<Program> program = parser.parse();

    SymbolTableVisitor symbolTableVisitor;
    program->accept(symbolTableVisitor);

    std::cout << "\033[33mUnused variables in this scope: ";
    for (const auto& name : symbolTableVisitor.symbolTable.unusedVariables)
    {
        std::cout << name << " ";
    }
    std::cout << "\033[0m" << std::endl;

    OptimizeVisitor optimizeVisitor(symbolTableVisitor.symbolTable);
    program->accept(optimizeVisitor);

    parser.toFile(".semantics", program);

    // TODO: code generation

    return EXIT_SUCCESS;
}
