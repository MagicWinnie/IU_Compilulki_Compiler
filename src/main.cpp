#include <iostream>

#include "lexical_analyzer/lexer.h"
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

    // TODO: semantics analyzer
    // TODO: code generation

    return EXIT_SUCCESS;
}
