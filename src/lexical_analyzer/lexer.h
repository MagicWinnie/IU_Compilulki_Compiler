#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#include "token.h"

class Lexer {
    std::string infile_path;
    std::ifstream infile;
    bool debug;
    static const int nKW = 15; // Number of keywords
    static const TokenCode Table[nKW]; // Table of keywords


public:
    explicit Lexer(const std::string &infile_path, const bool &debug);

    std::vector<std::unique_ptr<Token> > parse();

    void get_next_char(size_t *);

    ~Lexer();

    TokenCode getTokenCode();

    std::unique_ptr<Token> getKeywordToken(const std::string &buffer, const Span &span);
};

#endif //LEXER_H
