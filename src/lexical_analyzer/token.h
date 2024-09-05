#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum TokenType {
    RESERVED_WORD,
    DELIMITER,
    IDENTIFIER
};

class Token {
    TokenType type;
    std::string value;
    size_t start;
    size_t end;

public:
    explicit Token(TokenType type, const std::string &value, size_t start, size_t end);
};

#endif //TOKEN_H
