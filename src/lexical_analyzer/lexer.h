#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "token.h"

inline std::unordered_set<char> validTokens = {'.', ':', ',', '(', ')', '[', ']'};

inline std::unordered_map<char, TokenCode> tokenMap = {
    {'.', DOT},
    {':', COLON},
    {',', COMMA},
    {'(', LEFT_PAREN},
    {')', RIGHT_PAREN},
    {'[', LEFT_SQUARE_BRACKET},
    {']', RIGHT_SQUARE_BRACKET}
};

inline std::unordered_map<TokenCode, std::string> tokenCodeToString = {
    {PROGRAM, "PROGRAM"},
    {CLASS, "CLASS"},
    {EXTENDS, "EXTENDS"},
    {IS, "IS"},
    {END, "END"},
    {VAR, "VAR"},
    {METHOD, "METHOD"},
    {THIS, "THIS"},
    {WHILE, "WHILE"},
    {LOOP, "LOOP"},
    {IF, "IF"},
    {THEN, "THEN"},
    {ELSE, "ELSE"},
    {RETURN, "RETURN"},
    {COLON, "COLON"},
    {DOT, "DOT"},
    {COMMA, "COMMA"},
    {COLON_EQUAL, "COLON_EQUAL"},
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_SQUARE_BRACKET, "LEFT_SQUARE_BRACKET"},
    {RIGHT_SQUARE_BRACKET, "RIGHT_SQUARE_BRACKET"},
    {IDENTIFIER, "IDENTIFIER"},
    {REAL, "REAL"},
    {INTEGER, "INTEGER"},
    {BOOLEAN, "BOOLEAN"},
    {UNKNOWN, "UNKNOWN"}
};

class Lexer {
    std::string infile_path;
    std::ifstream infile;
    bool debug;

    void get_next_char(size_t *);

    static std::unique_ptr<Token> getKeywordToken(const std::string &buffer, const Span &span);

public:
    explicit Lexer(const std::string &infile_path, const bool &debug);

    std::vector<std::unique_ptr<Token> > parse();

    ~Lexer();
};

#endif //LEXER_H
