#include "./Token.hpp"
#include <cstddef>
#include <string>
#include <format>
#include "../superclass/super.hpp"


Token::Token(TokenType type, std::string& lexeme, super::object& literal, int line)
    : type(type),
      lexeme(lexeme),
      literal(literal),
      line(line) {}


std::string Token::tokenTypeToString(const TokenType type) const {

    switch (type) {

        case TokenType::LEFT_PAREN:    return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:   return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:    return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:   return "RIGHT_BRACE";
        case TokenType::COMMA:         return "COMMA";
        case TokenType::DOT:           return "DOT";
        case TokenType::MINUS:         return "MINUS";
        case TokenType::PLUS:          return "PLUS";
        case TokenType::SEMICOLON:     return "SEMICOLON";
        case TokenType::SLASH:         return "SLASH";
        case TokenType::STAR:          return "STAR";
        case TokenType::POWER:         return "POWER";
        case TokenType::MOD:           return "MOD";

        // One or two character tokens
        case TokenType::BANG:          return "BANG";
        case TokenType::NOT_EQUAL:     return "NOT_EQUAL";
        case TokenType::ASSIGN:        return "ASSIGN";
        case TokenType::EQUAL:         return "EQUAL";
        case TokenType::GREATER:       return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS:          return "LESS";
        case TokenType::LESS_EQUAL:    return "LESS_EQUAL";

        // Literals
        case TokenType::IDENTIFIER:    return "IDENTIFIER";
        case TokenType::STRING:        return "STRING";
        case TokenType::NUMBER:        return "NUMBER";

        // Keywords
        case TokenType::AND:           return "AND";
        case TokenType::AS:            return "AS";
        case TokenType::SUPER:         return "BASE";
        case TokenType::CLASS:         return "CLASS";
        case TokenType::DO:            return "DO";
        case TokenType::DEF:           return "DEF";
        case TokenType::ELSE:          return "ELSE";
        case TokenType::FALSE:         return "FALSE";
        case TokenType::FOR:           return "FOR";
        case TokenType::IF:            return "IF";
        case TokenType::JUMP:          return "JUMP";
        case TokenType::NIL:           return "NIL";
        case TokenType::OR:            return "OR";
        case TokenType::IOPUTF:        return "IOPUTF";
        case TokenType::IOPUTFN:        return "IOPUTFN";
        case TokenType::THIS:          return "THIS";
        case TokenType::TRUE:          return "TRUE";
        case TokenType::VAR:           return "VAR";
        case TokenType::WHILE:         return "WHILE";

        // End of File
        case TokenType::END_OF_FILE:   return "END_OF_FILE";

        default:                       return "UNSUPPORTED_TOKEN";
    }
}


std::string Token::toString(void) const {

    return this->tokenTypeToString(this->type) + " " + this->lexeme + " " + std::to_string(this->line);
}




    






