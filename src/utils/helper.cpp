//
// Created by nai1ka on 10/13/24.
//

#include <string>
#include <unordered_map>
#include "../lexical_analyzer/token_code.h"

inline std::unordered_map<TokenCode, std::string> tokenCodeToString = {
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

inline std::string getEnumName(const TokenCode code)
{
    const auto it = tokenCodeToString.find(code);
    if (it != tokenCodeToString.end())
    {
        return it->second; // Return the string if found
    }
    return "UNKNOWN_ENUM_VALUE"; // Fallback in case of unknown enum value
}
