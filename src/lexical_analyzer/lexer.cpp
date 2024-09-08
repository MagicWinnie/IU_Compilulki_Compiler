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
    std::vector<std::string> tempStrings;

    TokenCode tokCode = UNKNOWN;
    std::string buffer;

    int line_number = 1;
    int pos = 1;

    while (!this->infile.eof()) {
        char ch = infile.get();
        pos++;
        switch (ch) {
            case '/': // Skip comments
                std::getline(infile, buffer);
                line_number++;
                pos = 1;
                buffer.clear();
                break;
            case '\n' :
                break;
            case ' ':
                break;
            case EOF:
                break;
            case '.':
                tokCode = DELIMITER_DOT;
                break;
            case ':':
                tokCode = DELIMITER_COLON;
                break;
            case ',':
                tokCode = DELIMITER_COMMA;
                break;
            case '=': // = or ==
                tokCode = EQUAL;
                break;
            case '!': // ! or !=
                tokCode = EXCLAMATION;
                break;
            case '%': // % or %=
                tokCode = PERCENT;
                break;
            case '(':
                tokCode = LEFT_PAREN;
                break;
            case ')':
                tokCode = RIGHT_PAREN;
                break;
            default:
                buffer += ch;
                tokCode = getKeywordToken(buffer);
                if(tokCode!=UNKNOWN){
                    ch = 0;
                }
                break;
        }
        if (tokCode != UNKNOWN || ch == '\n' || ch == ' ' || ch == EOF) {
            if(!buffer.empty()){
                tokCode = IDENTIFIER;
                tokens.emplace_back(new Token(*new Span(line_number, pos-1-buffer.length(), pos-1), tokCode));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            }
            if(ch=='\n'){
                line_number++;
                pos = 1;

            }
            if(ch!=0 && ch!='\n' && ch!=' ' && ch!=EOF){
                tempStrings.emplace_back(string(1,ch));
                tokens.emplace_back(new Token(*new Span(line_number, pos-1,pos), tokCode));
            }
            tokCode = UNKNOWN;
        }

    }

    if(this->debug){
        for(int i=0;i<tokens.size();i++){
            std::cout << (tokens[i])->to_string() << " -> "<<tempStrings[i] << std::endl;
        }
    }


    return tokens;
}


TokenCode Lexer::getKeywordToken(const std::string &buffer) {
    // TODO add hash
    if (buffer == "while") return WHILE;
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
    else return UNKNOWN;
    // TODO finish this
}


Lexer::~Lexer() {
    this->infile.close();
}
