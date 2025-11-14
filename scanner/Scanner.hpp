#pragma once

#include "../tokeniser/Token.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include "../superclass/super.hpp"


class Scanner {

    private:
        
       inline static std::unordered_map<std::string, TokenType> keywords = {
            
            {"and",     TokenType::AND},
            {"as",      TokenType::AS},
            {"super",   TokenType::SUPER},
            {"class",   TokenType::CLASS},
            {"def",     TokenType::DEF},
            {"do",      TokenType::DO},
            {"else",    TokenType::ELSE},
            {"false",   TokenType::FALSE},
            {"for",     TokenType::FOR},
            {"if",      TokenType::IF},
            {"jump",    TokenType::JUMP},
            {"nil",     TokenType::NIL},
            {"or",      TokenType::OR},
            {"ioputf",  TokenType::IOPUTF},
            {"ioputfn", TokenType::IOPUTFN},
            {"then",    TokenType::THEN},
            {"this",    TokenType::THIS},
            {"true",    TokenType::TRUE},
            {"var",     TokenType::VAR},
            {"while",   TokenType::WHILE}
        };

        int start = 0;
        int current = 0;
        int line = 1;

        const std::string source;
        std::vector<Token*> tokens;

        void scanToken(void);
        bool isAtEnd(void);
        char advance(void);
        void addToken(TokenType type);
        void addToken(TokenType type, super::object& literal);
        bool match(char expected);
        char peek(void);
        char peekNext(void);
        void number(void);
        void identifier(void);
        void multilineComment(void);
        void str(void);

    public:

        Scanner(std::string& source);

        std::vector<Token*> scanTokens(void);

        ~Scanner();
};






