#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "token.h"

inline std::unordered_set validTokens = {'.', ':', ',', '(', ')', '[', ']'};

inline std::unordered_map<char, TokenCode> tokenMap = {
    {'.', DOT},
    {':', COLON},
    {',', COMMA},
    {'(', LEFT_PAREN},
    {')', RIGHT_PAREN},
    {'[', LEFT_SQUARE_BRACKET},
    {']', RIGHT_SQUARE_BRACKET}
};


class Lexer
{
    std::string infile_path;
    std::ifstream infile;
    bool debug;

    void getNextChar(size_t*);

    static std::unique_ptr<Token> getKeywordToken(const std::string&, const Span&);

public:
    explicit Lexer(const std::string&, const bool&);

    std::vector<std::unique_ptr<Token>> parse();

    ~Lexer();
};
