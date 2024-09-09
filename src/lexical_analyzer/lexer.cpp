#include "lexer.h"
#include "token.h"
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <iostream>

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
    {'.', DOT},
    {':', COLON},
    {',', COMMA},
    {'(', LEFT_PAREN},
    {')', RIGHT_PAREN},
    {'[', LEFT_SQUARE_BRACKET},
    {']', RIGHT_SQUARE_BRACKET},
    // Add more mappings as necessary
};

std::unordered_set<char> validTokens = {'.', ':', ',', '(', ')', '[', ']'};

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
            constexpr int nameWidth = 36;
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

        if (isdigit(next_char) && buffer.empty()) {
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
                    get_next_char(&pos);
                    if (infile.peek() == '=') {
                        tokens.emplace_back(
                            std::make_unique<Delimiter>(Span(line_number, pos - 1, pos + 1), COLON_EQUAL));
                        tempStrings.emplace_back(":=");
                        get_next_char(&pos);
                        continue;
                    }
                }

                TokenCode tokCode = tokenMap[next_char];
                tokens.emplace_back(std::make_unique<Delimiter>(Span(line_number, pos, pos + 1), tokCode));
                tempStrings.emplace_back(1, next_char);
                get_next_char(&pos);
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
    return std::make_unique<Keyword>(span, IDENTIFIER);
}


Lexer::~Lexer() {
    this->infile.close();
}
