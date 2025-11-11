#include "./Parser.hpp"
#include "../lexer/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../main/cscript.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include "../superclass/super.hpp"


Parser::Parser(std::vector<Token>& tokens) : tokens(tokens) {}


// older version
// Expr* Parser::unsafe_parse(void) {
//
//     try {
//
//         return this->unsafe_expression();
//
//     } catch (const Parser::ParseError&) {
//
//         return nullptr;
//     }
// }


std::vector<Stmt*> Parser::unsafe_parse(void) {

    try {

        std::vector<Stmt*> statements;

        while (!this->isAtEnd())
            statements.emplace_back(this->unsafe_statement());

        return statements;
    
    } catch (const Parser::ParseError&) {

        return std::vector<Stmt*>({});
    }
}


Expr* Parser::unsafe_expression(void) {

    return this->unsafe_equality();
}


Expr* Parser::unsafe_equality(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_comparison());

    while (this->match({TokenType::NOT_EQUAL, TokenType::EQUAL})) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_comparison());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_comparison(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_term());

    while (this->match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_term());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_term(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_factor());

    while (this->match({TokenType::MINUS, TokenType::PLUS})) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_factor());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_factor(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_power());

    while (this->match({TokenType::SLASH, TokenType::STAR, TokenType::MOD})) {

        Token operatr = this->previous();
        
        std::unique_ptr<Expr> right(this->unsafe_power());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_power(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_unary());

    while (this->match({TokenType::POWER})) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_power());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_unary(void) { // free memory later

    if (this->match({TokenType::BANG, TokenType::MINUS})) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_unary());

        return new Unary(operatr, std::move(right));
    }

    return this->unsafe_primary();
}


Expr* Parser::unsafe_primary(void) { // free memory later

    if (this->match({TokenType::FALSE}))
        return new Literal(false);

    if (this->match({TokenType::TRUE}))
        return new Literal(true);

    if (this->match({TokenType::NIL}))
        return new Literal(nullptr);
    
    if (this->match({TokenType::NUMBER, TokenType::STRING}))
        return new Literal(this->previous().literal);

    if (this->match({TokenType::LEFT_PAREN})) {

        std::unique_ptr<Expr> expr(this->unsafe_expression());

        std::string errMessage = "EXPECTED \')\' AFTER EXPRESSION";

        this->consume(TokenType::RIGHT_PAREN, errMessage);

        return new Grouping(std::move(expr));
    }

    std::string errMessage = "EXPECTED EXPRESSION";

    throw Parser::error(this->peek(), errMessage);
}


bool Parser::match(const std::vector<TokenType>& types) {

    for (const TokenType type : types) {

        if (this->check(type)) {

            this->advance();

            return true;
        }
    }

    return false;
}


bool Parser::check(TokenType type) {

    if (this->isAtEnd())
        return false;

    return this->peek().type == type;
}


Token Parser::advance(void) {

    if (!this->isAtEnd())
        this->current++;

    return this->previous();
}


bool Parser::isAtEnd(void) {

    return this->peek().type == TokenType::END_OF_FILE;
}


Token Parser::peek(void) {

    return this->tokens.at(this->current);
}


Token Parser::previous(void) {

    return this->tokens.at(this->current - 1);
}


Token Parser::consume(TokenType type, std::string& errMessage) {

    if (this->check(type))
        return this->advance();

    throw this->error(this->peek(), errMessage); // panic mode
}


Parser::ParseError::ParseError() : std::runtime_error("PARSER ERROR") {}


Parser::ParseError Parser::error(Token token, std::string& errMessage) {

    cscript::error(token, errMessage);

    return Parser::ParseError();
}


void Parser::synchronise(void) {

    this->advance();

    while (!this->isAtEnd()) {

        if (this->previous().type == TokenType::SEMICOLON)
            return;
    }

    switch (this->peek().type) {
    
        case TokenType::CLASS:
        case TokenType::DEF:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::IOPUTF:
        case TokenType::JUMP:

            return;

        default:
            break;
    }

    this->advance();

    return;
}


Stmt* Parser::unsafe_statement(void) { // free memory later

    if (this->match({TokenType::IOPUTF}))
        return this->unsafe_printStatement();

    return this->unsafe_expressionStatement();
}


Stmt* Parser::unsafe_printStatement(void) { // free memory later

    std::unique_ptr<Expr> val(this->unsafe_expression());

    std::string errMessage = "EXPECTED \';\' AFTER VALUE";

    this->consume(TokenType::SEMICOLON, errMessage);

    return new Print(std::move(val));
}


Stmt* Parser::unsafe_expressionStatement(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_expression());

    std::string errMessage = "EXPECTED \';\' AFTER EXPRESSION";

    this->consume(TokenType::SEMICOLON, errMessage);

    return new Expression(std::move(expr));
}






















