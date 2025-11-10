#pragma once


#include "../syntax/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include <stdexcept>
#include <string>
#include <vector>


class Parser {

    private:

        std::vector<Token> tokens;
        int current = 0;

        bool match(std::vector<TokenType>& types);
        bool check(TokenType type);
        bool isAtEnd(void);
        Token peek(void);
        Token advance(void);
        Token previous(void);

        Expr* unsafe_expression(void);
        Expr* unsafe_equality(void);
        Expr* unsafe_comparison(void);
        Expr* unsafe_term(void);
        Expr* unsafe_factor(void);
        Expr* unsafe_unary(void);
        Expr* unsafe_power(void);
        Expr* unsafe_primary(void);

        Token consume(TokenType type, std::string& errMessage);

        void synchronise(void);


        class ParseError : public std::runtime_error {

            public:

                ParseError();
        };

        ParseError error(Token token, std::string& errMessage);
    
    public:

        Parser(std::vector<Token>& tokens);

        Expr* unsafe_parse(void);

        ~Parser() = default;
};





