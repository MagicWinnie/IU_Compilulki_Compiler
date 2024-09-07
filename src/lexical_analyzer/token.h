#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <utility>

#include "span.h"
#include "token_code.h"


class Token {
    Span span;
    TokenCode code;

public:
    Token(const Span &span, const TokenCode code)
        : span(span),
          code(code) {
    }


    std::string to_string();
};

class Identifier : Token {
    std::string identifier;

public:
    Identifier(const Span &span, const TokenCode code, std::string identifier)
        : Token(span, code),
          identifier(std::move(identifier)) {
    }
};

class Integer : Token {
    long value;

public:
    Integer(const Span &span, const TokenCode code, const long value)
        : Token(span, code),
          value(value) {
    }
};

class Real : Token {
    long double value;

public:
    Real(const Span &span, const TokenCode code, const long double value)
        : Token(span, code),
          value(value) {
    }
};

#endif //TOKEN_H
