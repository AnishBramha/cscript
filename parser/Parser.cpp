#include "./Parser.hpp"
#include "../syntax/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../main/cscript.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "../superclass/super.hpp"


Parser::Parser(std::vector<Token>& tokens) : tokens(tokens) {}

Expr* Parser::unsafe_expression(void) {

    return this->unsafe_equality();
}

Expr* Parser::unsafe_equality(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_comparison());

    std::vector<TokenType> operators = {TokenType::NOT_EQUAL, TokenType::EQUAL};

    while (this->match(operators)) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_comparison());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


bool Parser::match(std::vector<TokenType>& types) {

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


Expr* Parser::unsafe_comparison(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_term());

    std::vector<TokenType> operators = {TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL};

    while (this->match(operators)) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_term());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_term(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_factor());

    std::vector<TokenType> operators = {TokenType::MINUS, TokenType::PLUS};

    while (this->match(operators)) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_factor());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_factor(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_unary());

    std::vector<TokenType> operators = {TokenType::SLASH, TokenType::STAR};

    while (this->match(operators)) {

        Token operatr = this->previous();
        
        std::unique_ptr<Expr> right(this->unsafe_unary());

        expr = std::make_unique<Binary>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_unary(void) { // free memory later

    std::vector<TokenType> operators = {TokenType::BANG, TokenType::MINUS};

    if (this->match(operators)) {

        Token operatr = this->previous();

        std::unique_ptr<Expr> right(this->unsafe_unary());

        return new Unary(operatr, std::move(right));
    }

    return this->unsafe_primary();
}


Expr* Parser::unsafe_primary(void) { // free memory later

    std::vector<TokenType> t1 = {TokenType::FALSE}, t2 = {TokenType::TRUE}, t3 = {TokenType::NIL}, t4 = {TokenType::NUMBER, TokenType::STRING}, t5 = {TokenType::LEFT_PAREN};

    object literal;

    if (this->match(t1)) {

        literal = false;

        return new Literal(literal);

    } if (this->match(t2)) {

        literal = true;

        return new Literal(literal);

    } if (this->match(t3)) {

        literal = nullptr;

        return new Literal(literal);
    
    } if (match(t4)) {

        literal = this->previous().literal;

        return new Literal(literal);

    } if (match(t5)) {

        std::unique_ptr<Expr> expr(this->unsafe_expression());

        std::string errMessage = "EXPECTED \')\' AFTER EXPRESSION";

        this->consume(TokenType::RIGHT_PAREN, errMessage);

        return new Grouping(std::move(expr));
    }

    std::string errMessage = "EXPECTED EXPRESSION";

    throw Parser::error(this->peek(), errMessage);
}


Token Parser::consume(TokenType type, std::string& errMessage) {

    if (this->check(type))
        return this->advance();

    throw this->error(this->peek(), errMessage);
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


Expr* Parser::unsafe_parse(void) {

    try {

        return this->unsafe_expression();
    
    } catch (const Parser::ParseError&) {

        return nullptr;
    }
}
























