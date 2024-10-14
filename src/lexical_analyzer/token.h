#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <utility>

#include "span.h"
#include "token_code.h"


class Token {
protected:
    Span span;
    TokenCode code;

public:
    virtual ~Token() = default;

    Token(const Span &span, const TokenCode code)
        : span(span),
          code(code) {
    }

    virtual std::string to_string();

    [[nodiscard]] TokenCode get_code() const;
    [[nodiscard]] Span get_span() const;
};

class Keyword final : public Token {
public:
    Keyword(const Span &span, const TokenCode code) : Token(span, code) {
    }

    std::string to_string() override;
};

class Identifier final : public Token {
protected:
    std::string identifier;

public:
    Identifier(const Span &span, const TokenCode code, std::string identifier)
        : Token(span, code),
          identifier(std::move(identifier)) {
    }

    [[nodiscard]] std::string get_identifier() const;

    std::string to_string() override;
};

class Integer final : public Token {
protected:
    long value;

public:
    Integer(const Span &span, const TokenCode code, const long value)
        : Token(span, code),
          value(value) {
    }

    std::string to_string() override;
};

class Real final : public Token {
protected:
    long double value;

public:
    Real(const Span &span, const TokenCode code, const long double value)
        : Token(span, code),
          value(value) {
    }

    std::string to_string() override;
};

class Boolean final : public Token {
protected:
    bool value;

public:
    Boolean(const Span &span, const TokenCode code, const bool value)
        : Token(span, code),
          value(value) {
    }

    std::string to_string() override;
};

class Delimiter final : public Token {
public:
    Delimiter(const Span &span, const TokenCode code)
        : Token(span, code) {
    }

    std::string to_string() override;
};

#endif //TOKEN_H
