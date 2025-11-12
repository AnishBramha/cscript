#pragma once


#include "../lexer/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../lexer/Stmt.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>


class Parser {

    private:

        std::vector<Token> tokens;
        int current = 0;

        bool match(const std::vector<TokenType>& types);
        bool check(TokenType type);
        Token peek(void);
        Token advance(void);
        Token previous(void);

        Expr* unsafe_assignment(void);
        Expr* unsafe_or(void);
        Expr* unsafe_and(void);
        Expr* unsafe_equality(void);
        Expr* unsafe_comparison(void);
        Expr* unsafe_term(void);
        Expr* unsafe_factor(void);
        Expr* unsafe_power(void);
        Expr* unsafe_unary(void);
        Expr* unsafe_primary(void);

        Token consume(TokenType type, std::string& errMessage);

        Stmt* unsafe_statement(void);
        Stmt* unsafe_forStatement(void);
        Stmt* unsafe_ifStatement(void);
        Stmt* unsafe_printStatement(void);
        Stmt* unsafe_printlnStatement(void);
        Stmt* unsafe_expressionStatement(void);
        Stmt* unsafe_declaration(void);
        Stmt* unsafe_varDeclaration(void);
        Stmt* unsafe_whileStatement(void);

        std::vector<std::unique_ptr<Stmt>> unsafe_block(void);

        void synchronise(void);


    public:

        Parser(std::vector<Token>& tokens);

        bool isAtEnd(void);

        std::vector<Stmt*> unsafe_parse(void);
        Expr* unsafe_expression(void);


        class ParseError : public std::runtime_error {

            public:

                ParseError();
        };

        ~Parser() = default;

    private:

        ParseError error(Token token, std::string& errMessage);
};





