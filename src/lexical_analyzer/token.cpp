#include "token.h"

Token::Token(const TokenType type, const std::string &value, const size_t start, const size_t end) {
    this->type = type;
    this->value = value;
    this->start = start;
    this->end = end;
}
