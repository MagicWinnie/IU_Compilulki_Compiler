#include "lexer.h"
#include "token.h"
#include <iomanip>
#include <iostream>
#include "../utils/helper.cpp"

Lexer::Lexer(const std::string &infile_path, const bool &debug) {
    this->infile_path = infile_path;
    this->infile.open(infile_path, std::ios_base::in);
    if (!this->infile.is_open()) {
        std::cerr << "Failed to open file " << infile_path << std::endl;
        exit(EXIT_FAILURE);
    }

    this->debug = debug;
}



void Lexer::getNextChar(size_t *pos) {
    infile.get();
    (*pos)++;
}

void printDebugInfo(const std::vector<std::unique_ptr<Token> > &tokens, const std::vector<std::string> &tempStrings,
                    std::ostream &out, const std::pair<std::string, std::string> &color) {
    if (!color.first.empty()) {
        out << color.first << "DEBUG" << color.second << std::endl;
        out << color.first << "---------------------------------------------" << color.second << std::endl;
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        constexpr int nameWidth = 46;
        std::string tokenStr = tokens[i]->to_string();
        std::string enumName = getEnumName(tokens[i]->get_code());

        out << color.first << std::setw(nameWidth) << std::left << tokenStr
                << " -> " << std::setw(nameWidth / 2) << std::left << tempStrings[i]
                << " -> " << std::setw(nameWidth) << std::left << enumName
                << color.second << std::endl;
    }

    if (!color.first.empty()) {
        out << color.first << "---------------------------------------------" << color.second << std::endl;
    }
}

std::vector<std::unique_ptr<Token> > Lexer::parse() {
    std::vector<std::unique_ptr<Token> > tokens;
    std::vector<std::string> tempStrings;

    std::string buffer;

    size_t line_number = 1;
    size_t pos = 0;

    while (!this->infile.eof()) {
        char next_char = static_cast<char>(infile.peek());

        if (isdigit(next_char) && buffer.empty()) {
            // Handle integer and real numbers
            getNextChar(&pos);
            buffer += next_char;

            while (isdigit(next_char = static_cast<char>(infile.peek()))) {
                buffer += next_char;
                getNextChar(&pos);
            }
            if (next_char == '.') {
                buffer += next_char;
                getNextChar(&pos);
                while (isdigit(next_char = static_cast<char>(infile.peek()))) {
                    buffer += next_char;
                    getNextChar(&pos);
                }
                tokens.emplace_back(std::make_unique<Real>(Span(line_number, pos - buffer.length(), pos), REAL,
                                                           std::stod(buffer)));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            } else {
                tokens.emplace_back(std::make_unique<Integer>(Span(line_number, pos - buffer.length(), pos), INTEGER,
                                                              std::stoi(buffer)));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            }
        } else if (validTokens.find(next_char) != validTokens.end()) {
            // Handle case when buffer is not empty, but the current character is a valid token
            if (!buffer.empty()) {
                tokens.emplace_back(getKeywordToken(buffer, Span(line_number, pos - buffer.length(), pos)));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            } else {
                if (next_char == ':') {
                    getNextChar(&pos);
                    if (infile.peek() == '=') {
                        tokens.emplace_back(
                            std::make_unique<Delimiter>(Span(line_number, pos - 1, pos + 1), COLON_EQUAL));
                        tempStrings.emplace_back(":=");
                        getNextChar(&pos);
                    } else {
                        tokens.emplace_back(std::make_unique<Delimiter>(Span(line_number, pos - 1, pos), COLON));
                        tempStrings.emplace_back(1, next_char);
                        getNextChar(&pos);
                    }
                    continue;
                }

                TokenCode tokCode = tokenMap[next_char];
                tokens.emplace_back(std::make_unique<Delimiter>(Span(line_number, pos, pos + 1), tokCode));
                tempStrings.emplace_back(1, next_char);
                getNextChar(&pos);
            }
        } else if (next_char == '\n' || isspace(next_char) || next_char == EOF) {
            if (!buffer.empty()) {
                tokens.emplace_back(getKeywordToken(buffer, Span(line_number, pos - buffer.length(), pos)));

                tempStrings.emplace_back(buffer);
                buffer.clear();
            }
            if (next_char == '\n') {
                line_number++;
                pos = 0;
            }
            getNextChar(&pos);
        } else if (next_char == '/') {
            // Skip the comment line
            std::getline(infile, buffer); // Skip the rest of the line
            line_number++;
            pos = 1;
            buffer.clear();
        } else {
            // Handle buffer (identifier or keyword)
            buffer += next_char;
            getNextChar(&pos);
        }
    }

    if (this->debug) {
        printDebugInfo(tokens, tempStrings, std::cout, std::make_pair("\033[1;33m", "\033[0m"));

        std::ofstream output_file;

        std::filesystem::path path(this->infile_path);
        path.replace_extension(".lexer");

        output_file.open(path);
        printDebugInfo(tokens, tempStrings, output_file, std::make_pair("", ""));
        output_file.close();
    }

    return tokens;
}


std::unique_ptr<Token> Lexer::getKeywordToken(const std::string &buffer, const Span &span) {
    if (buffer == "while") return std::make_unique<Keyword>(span, WHILE);
    if (buffer == "var") return std::make_unique<Keyword>(span, VAR);
    if (buffer == "method") return std::make_unique<Keyword>(span, METHOD);
    if (buffer == "this") return std::make_unique<Keyword>(span, THIS);
    if (buffer == "loop") return std::make_unique<Keyword>(span, LOOP);
    if (buffer == "if") return std::make_unique<Keyword>(span, IF);
    if (buffer == "then") return std::make_unique<Keyword>(span, THEN);
    if (buffer == "else") return std::make_unique<Keyword>(span, ELSE);
    if (buffer == "return") return std::make_unique<Keyword>(span, RETURN);
    if (buffer == "Program") return std::make_unique<Keyword>(span, PROGRAM);
    if (buffer == "class") return std::make_unique<Keyword>(span, CLASS);
    if (buffer == "extends") return std::make_unique<Keyword>(span, EXTENDS);
    if (buffer == "is") return std::make_unique<Keyword>(span, IS);
    if (buffer == "end") return std::make_unique<Keyword>(span, END);
    if (buffer == "true") return std::make_unique<Boolean>(span, BOOLEAN, true);
    if (buffer == "false") return std::make_unique<Boolean>(span, BOOLEAN, false);
    return std::make_unique<Identifier>(span, IDENTIFIER, buffer);
}


Lexer::~Lexer() {
    this->infile.close();
}
