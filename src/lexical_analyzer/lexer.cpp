#include "lexer.h"
#include "token.h"
#include <unordered_map>
#include <unordered_set>
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
        {'!', EXCLAMATION},
        {'%', PERCENT},
        {'(', LEFT_PAREN},
        {')', RIGHT_PAREN},
        // Add more mappings as necessary
};

std::unordered_set<char> validTokens = {'.', ':', ',', '=', '!', '%', '(', ')'};

std::string getEnumName(TokenCode code) {
    std::unordered_map<TokenCode, std::string> tokenCodeToString = {
            {PROGRAM,         "PROGRAM"},
            {CLASS,           "CLASS"},
            {EXTENDS,         "EXTENDS"},
            {IS,              "IS"},
            {END,             "END"},
            {VAR,             "VAR"},
            {METHOD,          "METHOD"},
            {THIS,            "THIS"},
            {WHILE,           "WHILE"},
            {LOOP,            "LOOP"},
            {IF,              "IF"},
            {THEN,            "THEN"},
            {ELSE,            "ELSE"},
            {RETURN,          "RETURN"},
            {DELIMITER_COLON, "DELIMITER_COLON"},
            {DELIMITER_DOT,   "DELIMITER_DOT"},
            {DELIMITER_COMMA, "DELIMITER_COMMA"},
            {EQUAL,           "EQUAL"},
            {EQUAL_EQUAL,     "EQUAL_EQUAL"},
            {NOT_EQUAL,       "NOT_EQUAL"},
            {EXCLAMATION,     "EXCLAMATION"},
            {PERCENT,         "PERCENT"},
            {PERCENT_EQUAL,   "PERCENT_EQUAL"},
            {LEFT_PAREN,      "LEFT_PAREN"},
            {RIGHT_PAREN,     "RIGHT_PAREN"},
            {IDENTIFIER,      "IDENTIFIER"},
            {REAL,            "REAL"},
            {INTEGER,         "INTEGER"},
            {UNKNOWN,         "UNKNOWN"}
    };
    auto it = tokenCodeToString.find(code);
    if (it != tokenCodeToString.end()) {
        return it->second;  // Return the string if found
    }
    return "UNKNOWN_ENUM_VALUE";  // Fallback in case of unknown enum value
}

void Lexer::get_next_char(int *pos) {
    infile.get();
    (*pos)++;
}
void printDebugInfo(const std::vector<std::unique_ptr<Token>>& tokens, const std::vector<std::string>& tempStrings, bool debug) {
    if (debug) {
        std::cout << "\033[1;33mDEBUG\033[0m" << std::endl;
        std::cout << "\033[1;33m---------------------------------------------\033[0m" << std::endl;
        const int nameWidth = 30;

        for (size_t i = 0; i < tokens.size(); ++i) {
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

std::vector<std::unique_ptr<Token>> Lexer::parse() {
    std::vector<std::unique_ptr<Token>> tokens;
    std::vector<std::string> tempStrings;

    TokenCode tokCode;
    std::string buffer;

    int line_number = 1;
    int pos = 0;

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
            if (buffer.length() > 0) {
                tokens.emplace_back(std::make_unique<Token>(Span(line_number, pos - buffer.length(), pos),
                                                            getKeywordToken(buffer)));
                tempStrings.emplace_back(buffer);
                buffer.clear();
            } else {
                tokCode = tokenMap[next_char];
                tokens.emplace_back(std::make_unique<Token>(Span(line_number, pos, pos + 1), tokCode));
                tempStrings.emplace_back(string(1, next_char));
                get_next_char(&pos);
            }

        } else if (next_char == '\n' || next_char == ' ') {
            if (buffer.length() > 0) {
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
            continue;
        } else if (next_char == '/') {
            // Skip the comment line
            std::getline(infile, buffer);  // Skip the rest of the line
            line_number++;
            pos = 0;
            buffer.clear();
            get_next_char(&pos);
            continue;
        } else {
            // Handle buffer (identifier or keyword)
            buffer += next_char;
            get_next_char(&pos);
            continue;
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
    else return IDENTIFIER;
    // TODO finish this
}


Lexer::~Lexer() {
    this->infile.close();
}
