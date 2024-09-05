#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>

class Lexer {
    std::ifstream infile;
    bool debug;

public:
    explicit Lexer(const std::string &infile_path, const bool &debug);

    void parse();

    ~Lexer();
};

#endif //LEXER_H
