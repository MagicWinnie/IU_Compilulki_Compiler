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

std::vector<std::unique_ptr<Token>> Lexer::parse() {
    std::vector<std::unique_ptr<Token>> tokens;
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
        if(tokCode!=UNKNOWN){
            cout<<buffer<<endl;
            buffer.clear();
            tokens.emplace_back(new Token(*new Span(line_number,start,end),tokCode));
            tokCode = UNKNOWN;
            start = end;
        }

    }
    for (auto &token : tokens) {
            std::cout << (*token).to_string() << std::endl;

    }
    return tokens;
}



TokenCode Lexer::getKeywordToken(const std::string &buffer) {
    // TODO add hash
    if (buffer == "switch") return WHILE;
    else if (buffer == "while") return WHILE;
    else if (buffer == "var") return VAR;
    else if (buffer == "method") return METHOD;
    else if (buffer == "this") return THIS;
    else if (buffer == "loop") return LOOP;
    else if (buffer == "if") return IF;
    else if (buffer == "then") return THEN;
    else if (buffer == "else") return ELSE;
    else if (buffer == "return") return RETURN;
    else if (buffer == "Program") return PROGRAM;
    else if (buffer == "class") return CLASS;
    else if (buffer == "extends") return EXTENDS;
    else if (buffer == "is") return IS;
    else if (buffer == "end") return END;
    else return UNKNOWN ;
    // TODO finish this
}


Lexer::~Lexer() {
    this->infile.close();
}
