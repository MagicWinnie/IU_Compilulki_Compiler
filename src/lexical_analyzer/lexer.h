#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>

class Lexer {
    std::ifstream infile;

public:
    explicit Lexer(const std::string &infile_path);

    ~Lexer();
};

#endif //LEXER_H
