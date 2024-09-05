#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>
#include <vector>

#include "token.h"

class Lexer {
    std::ifstream infile;
    bool debug;

public:
    explicit Lexer(const std::string &infile_path, const bool &debug);

    std::vector<Token> parse();

    ~Lexer();
};

#endif //LEXER_H
