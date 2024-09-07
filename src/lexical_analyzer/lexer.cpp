#include "lexer.h"
#include "token.h"

#include <iostream>

using namespace std;
//const TokenCode Lexer::Table[Lexer::nKW] = {
//        PROGRAM, CLASS, EXTENDS, IS, END, VAR, METHOD, THIS, WHILE, LOOP, IF, THEN, ELSE, RETURN, DELIMITER_COLON
//};

Lexer::Lexer(const std::string &infile_path, const bool &debug) {
    this->infile.open(infile_path, std::ios_base::in);
    if (!this->infile.is_open()) {
        std::cerr << "Failed to open file " << infile_path << std::endl;
        exit(EXIT_FAILURE);
    }

    this->debug = debug;
}

std::vector<std::unique_ptr<Token> > Lexer::parse() {
    std::vector<std::unique_ptr<Token> > tokens;
    TokenCode tokCode = UNKNOWN;
    std::string buffer;

    int line_number = 0;
    int start = 0;
    int end = 0;

    while (!this->infile.eof()) {
        char ch = infile.get();
        end++;
        switch (ch) {
            case ' ': // space
                buffer.clear();
                tokCode = UNKNOWN;
                break;
            case '\n': // new line
                line_number++;
                tokCode = UNKNOWN;
                start = 0;
                end = 0;
                buffer.clear();
                break;
            case '.':
                tokCode = DELIMITER_DOT;
                break;
            case ':':
                tokCode = DELIMITER_COLON;
                break;
            case '=': // = or ==
                if (infile.get() == '=')
                    tokCode = EQUAL_EQUAL;
                else
                    tokCode = EQUAL;
                break;
            case '!': // ! or !=
                if (infile.get() == '=') {
                    tokCode = NOT_EQUAL;
                } else {
                    tokCode = EXCLAMATION;
                }
                break;
            case '%': // % or %=
                if (infile.get() == '=')
                    tokCode = PERCENT_EQUAL;
                else
                    tokCode = PERCENT;
                break;

            default:
                buffer += ch;
                tokCode = getKeywordToken(buffer);
                break;
        }
        if (tokCode != UNKNOWN) {
            cout << buffer << endl;
            buffer.clear();
            tokens.emplace_back(new Token(Span(line_number, start, end), tokCode));
            tokCode = UNKNOWN;
            start = end;
        }
    }
    return tokens;
}


TokenCode Lexer::getKeywordToken(const std::string &buffer) {
    // TODO add hash
    if (buffer == "switch") return WHILE;
    if (buffer == "while") return WHILE;
    if (buffer == "var") return VAR;
    if (buffer == "method") return METHOD;
    if (buffer == "this") return THIS;
    if (buffer == "loop") return LOOP;
    if (buffer == "if") return IF;
    if (buffer == "then") return THEN;
    if (buffer == "else") return ELSE;
    if (buffer == "return") return RETURN;
    if (buffer == "Program") return PROGRAM;
    if (buffer == "class") return CLASS;
    if (buffer == "extends") return EXTENDS;
    if (buffer == "is") return IS;
    if (buffer == "end") return END;
    return UNKNOWN;
    // TODO finish this
}


Lexer::~Lexer() {
    this->infile.close();
}
