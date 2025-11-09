#pragma once

#include "./TokenType.hpp"
#include "../superclass/super.hpp"
#include <cstddef>
#include <string>


class Token {

    private:


    public:

        const TokenType type;
        const std::string lexeme;
        const object literal;
        const int line;

        Token(TokenType type, std::string& lexeme, object& literal, int line);

        std::string tokenTypeToString(const TokenType type) const;
        std::string objectToString(const object& obj) const;
        std::string toString(void) const;

        ~Token() = default;
};





