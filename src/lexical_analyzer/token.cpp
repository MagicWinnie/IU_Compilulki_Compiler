//
// Created by Nail Minnemullin on 07.09.2024.
//

#include "token.h"

std::string Token::to_string() {
    return "Token{" + span.to_string() + ", " + std::to_string(code) + "}";
}