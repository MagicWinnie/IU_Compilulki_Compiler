#include "lexer.h"
#include "token.h"
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
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

std::unordered_map<char, TokenCode> tokenMap = {
    {'.', DELIMITER_DOT},
    {':', DELIMITER_COLON},
    {',', DELIMITER_COMMA},
    {'=', EQUAL},
    {'(', LEFT_PAREN},
    {')', RIGHT_PAREN},
    // Add more mappings as necessary
};

std::unordered_set<char> validTokens = {'.', ':', ',', '=', '(', ')'};

std::string getEnumName(TokenCode code) {
    std::unordered_map<TokenCode, std::string> tokenCodeToString = {
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
        {DELIMITER_COLON, "DELIMITER_COLON"},
        {DELIMITER_DOT, "DELIMITER_DOT"},
        {DELIMITER_COMMA, "DELIMITER_COMMA"},
        {EQUAL, "EQUAL"},
        {LEFT_PAREN, "LEFT_PAREN"},
        {RIGHT_PAREN, "RIGHT_PAREN"},
        {IDENTIFIER, "IDENTIFIER"},
        {REAL, "REAL"},
        {INTEGER, "INTEGER"},
        {UNKNOWN, "UNKNOWN"}
    };
    auto it = tokenCodeToString.find(code);
    if (it != tokenCodeToString.end()) {
        return it->second; // Return the string if found
    }
    return "UNKNOWN_ENUM_VALUE"; // Fallback in case of unknown enum value
}

void Lexer::get_next_char(size_t *pos) {
    infile.get();
    (*pos)++;
}

void printDebugInfo(const std::vector<std::unique_ptr<Token> > &tokens, const std::vector<std::string> &tempStrings,
                    bool debug) {
    if (debug) {
        std::cout << "\033[1;33mDEBUG\033[0m" << std::endl;
        std::cout << "\033[1;33m---------------------------------------------\033[0m" << std::endl;

        for (size_t i = 0; i < tokens.size(); ++i) {
            constexpr int nameWidth = 30;
            std::string tokenStr = tokens[i]->to_string();
            std::string enumName = getEnumName(tokens[i]->get_code());

            std::cout << "\033[1;33m" << std::setw(nameWidth) << std::left << tokenStr
                    << " -> " << std::setw(nameWidth) << std::left << tempStrings[i]
                    << " -> " << std::setw(nameWidth) << std::left << enumName
                    << "\033[0m" << std::endl;
        }
        std::cout << "\033[1;33m---------------------------------------------\033[0m" << std::endl;
    }
}

std::vector<std::unique_ptr<Token> > Lexer::parse() {
    std::vector<std::unique_ptr<Token> > tokens;
    std::vector<std::string> tempStrings;

    std::string buffer;

    size_t line_number = 1;
    size_t pos = 0;

    while (!this->infile.eof()) {
        char next_char = infile.peek();

        if (isdigit(next_char)) {
            // Handle integer and real numbers
            get_next_char(&pos);
            buffer += next_char;

            while (isdigit(next_char = infile.peek())) {
                buffer += next_char;
                get_next_char(&pos);
            }
            if (next_char == '.') {
                buffer += next_char;
                get_next_char(&pos);
                while (isdigit(next_char = infile.peek())) {
                    buffer += next_char;
                    get_next_char(&pos);
                }
                tokens.emplace_back(std::make_unique<Token>(Span(line_number, pos - buffer.length(), pos), REAL));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            } else {
                tokens.emplace_back(std::make_unique<Token>(Span(line_number, pos - buffer.length(), pos), INTEGER));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            }
        } else if (validTokens.find(next_char) != validTokens.end()) {
            // Handle case when buffer is not empty, but the current character is a valid token
            if (!buffer.empty()) {
                tokens.emplace_back(std::make_unique<Token>(Span(line_number, pos - buffer.length(), pos),
                                                            getKeywordToken(buffer)));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            } else {
                TokenCode tokCode = tokenMap[next_char];
                tokens.emplace_back(std::make_unique<Token>(Span(line_number, pos, pos + 1), tokCode));
                tempStrings.emplace_back(1, next_char);
                get_next_char(&pos);
            }
        } else if (next_char == '\n' || isspace(next_char) || next_char == EOF) {
            if (!buffer.empty()) {
                tokens.emplace_back(std::make_unique<Token>(Span(line_number, pos - buffer.length(), pos),
                                                            getKeywordToken(buffer)));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            }
            if (next_char == '\n') {
                line_number++;
                pos = 0;
            }
            get_next_char(&pos);
        } else if (next_char == '/') {
            // Skip the comment line
            std::getline(infile, buffer); // Skip the rest of the line
            line_number++;
            pos = 0;
            buffer.clear();
        } else {
            // Handle buffer (identifier or keyword)
            buffer += next_char;
            get_next_char(&pos);
        }
    }

    if (this->debug) {
        printDebugInfo(tokens, tempStrings, this->debug);
    }

    return tokens;
}


TokenCode Lexer::getKeywordToken(const std::string &buffer) {
    // TODO add hash
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
    return IDENTIFIER;
    // TODO finish this
}


Lexer::~Lexer() {
    this->infile.close();
}
