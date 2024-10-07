//
// Created by Nail Minnemullin on 07.09.2024.
//

#include "token.h"

std::string Token::to_string() {
    return "Token{" + span.to_string() + ", " + std::to_string(code) + "}";
}

TokenCode Token::get_code() const {
    return code;
}

std::string Identifier::get_identifier() const {
    return identifier;
}

std::string Keyword::to_string() {
    return "Keyword{" + span.to_string() + ", " + std::to_string(code) + "}";
}

std::string Identifier::to_string() {
    return "Identifier{" + span.to_string() + ", " + std::to_string(code) + ", " + identifier + "}";
}

std::string Integer::to_string() {
    return "Integer{" + span.to_string() + ", " + std::to_string(code) + ", " + std::to_string(value) + "}";
}

std::string Real::to_string() {
    return "Real{" + span.to_string() + ", " + std::to_string(code) + ", " + std::to_string(value) + "}";
}

std::string Boolean::to_string() {
    return "Boolean{" + span.to_string() + ", " + std::to_string(code) + ", " + std::to_string(value) + "}";
}

std::string Delimiter::to_string() {
    return "Delimiter{" + span.to_string() + ", " + std::to_string(code) + "}";
}
