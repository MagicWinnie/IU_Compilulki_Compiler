#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "token.h"

class Lexer {
    std::ifstream infile;
    bool debug;
    static const int nKW = 15; // Number of keywords
    static const TokenCode Table[nKW]; // Table of keywords


public:
    explicit Lexer(const std::string &infile_path, const bool &debug);

    std::vector<std::unique_ptr<Token>> parse();

    ~Lexer();

    TokenCode getTokenCode();

    TokenCode getKeywordToken(const std::string &buffer);
};

#endif //LEXER_H
