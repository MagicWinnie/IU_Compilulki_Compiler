#include <iostream>

#include "lexical_analyzer/lexer.h"
#include "syntax_analyzer/Parser.h"
#include "settings/settings.h"


int main(const int argc, char *argv[]) {
    auto settings = Settings();
    settings.parse(argc, argv);
    settings.process();

    std::cout << settings << std::endl;

    auto lexer = Lexer(settings.get_infile(), settings.get_debug());
    auto tokens = lexer.parse();

    auto parser = Parser(std::move(tokens));
    parser.parse();


    // TODO: semantics analyzer
    // TODO: code generation

    return EXIT_SUCCESS;
}
