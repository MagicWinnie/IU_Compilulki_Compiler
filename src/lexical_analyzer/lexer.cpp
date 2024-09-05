#include "lexer.h"

#include <iostream>


Lexer::Lexer(const std::string &infile_path, const bool &debug) {
    this->infile.open(infile_path, std::ios_base::in);
    if (!this->infile.is_open()) {
        std::cerr << "Failed to open file " << infile_path << std::endl;
        exit(EXIT_FAILURE);
    }

    this->debug = debug;
}

void Lexer::parse() {
}

Lexer::~Lexer() {
    this->infile.close();
}
