#include "./Parser.hpp"
#include "../lexer/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../main/cscript.hpp"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include "../superclass/super.hpp"


Parser::Parser(std::vector<Token>& tokens) : tokens(tokens) {}


std::vector<Stmt*> Parser::unsafe_parse(void) {

    try {

        std::vector<Stmt*> statements;

        while (!this->isAtEnd())
            statements.emplace_back(this->unsafe_declaration());

        return statements;
    
    } catch (const Parser::ParseError&) {

        return std::vector<Stmt*>({});
    }
}


Expr* Parser::unsafe_expression(void) {

    return this->unsafe_assignment();
}


Expr* Parser::unsafe_assignment(void) {

    std::unique_ptr<Expr> expr(this->unsafe_or());

    if (this->match({TokenType::ASSIGN})) {

        Token assign = this->previous();
        std::unique_ptr<Expr> val(this->unsafe_assignment());

        auto* var = dynamic_cast<Variable*>(expr.get());

        if (var) {

            Token name = var->name;

            return new Assign(name, std::move(val));
        }

        std::string errMessage = "INVALID ASSIGNMENT TARGET";
        cscript::error(assign, errMessage);
    }

    return expr.release();
}


Expr* Parser::unsafe_or(void) {

    std::unique_ptr<Expr> expr(this->unsafe_and());

    while (this->match({TokenType::OR})) {

        Token operatr = this->previous();
        std::unique_ptr<Expr> right(this->unsafe_and());
        expr = std::make_unique<Logical>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
}


Expr* Parser::unsafe_and(void) {

    std::unique_ptr<Expr> expr(this->unsafe_equality());

    while (this->match({TokenType::AND})) {

        Token operatr = this->previous();
        std::unique_ptr<Expr> right(this->unsafe_equality());
        expr = std::make_unique<Logical>(std::move(expr), operatr, std::move(right));
    }

    return expr.release();
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

    return this->unsafe_call();
}


Expr* Parser::unsafe_call(void) { // free memory later
    
    std::unique_ptr<Expr> expr(this->unsafe_primary());

    for (;;) {

        if (this->match({TokenType::LEFT_PAREN}))
            expr.reset(this->unsafe_finishCall(std::move(expr)));

        else
            break;
    }

    return expr.release();
}


Expr* Parser::unsafe_finishCall(std::unique_ptr<Expr> callee) {

    std::vector<std::unique_ptr<Expr>> args;

    if (!this->check(TokenType::RIGHT_PAREN)) {

        do {

            if (args.size() >= 255) {

                std::string errMessage = "CANNOT HAVE MORE THAN 255 FUNCTION ARGUMENTS";
                cscript::error(this->peek(), errMessage);
            }

            args.emplace_back(this->unsafe_expression());

        } while (this->match({TokenType::COMMA}));
    }

    std::string errMessage = "EXPECTED \')\' AFTER FUNCTION ARGUMENTS";
    Token paren = this->consume(TokenType::RIGHT_PAREN, errMessage);

    return new Call(std::move(callee), paren, std::move(args));
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

    if (this->match({TokenType::IDENTIFIER}))
        return new Variable(this->previous());

    if (this->match({TokenType::LEFT_PAREN})) {

        std::unique_ptr<Expr> expr(this->unsafe_expression());

        std::string errMessage = "EXPECTED \')\' AFTER EXPRESSION";
        this->consume(TokenType::RIGHT_PAREN, errMessage);

        return new Grouping(std::move(expr));
    }

    std::string errMessage = "EXPECTED EXPRESSION";

    throw Parser::error(this->peek(), errMessage);
}


Stmt* Parser::unsafe_statement(void) { // free memory later

    if (this->match({TokenType::FOR}))
        return this->unsafe_forStatement();

    if (this->match({TokenType::IF}))
        return this->unsafe_ifStatement();

    if (this->match({TokenType::IOPUTF}))
        return this->unsafe_printStatement();

    if (this->match({TokenType::IOPUTFN}))
        return this->unsafe_printlnStatement();

    if (this->match({TokenType::WHILE}))
        return this->unsafe_whileStatement();

    if (this->match({TokenType::LEFT_BRACE}))
        return new Block(this->unsafe_block());

    return this->unsafe_expressionStatement();
}


Stmt* Parser::unsafe_forStatement(void) {

    std::string errMessage = "EXPECTED \'(\' AFTER \'FOR\'";
    this->consume(TokenType::LEFT_PAREN, errMessage);

    std::unique_ptr<Stmt> initialiser;

    if (this->match({TokenType::SEMICOLON}))
        initialiser = nullptr;

    else if (this->match({TokenType::VAR}))
        initialiser.reset(this->unsafe_varDeclaration());

    else
        initialiser.reset(this->unsafe_expressionStatement());


    std::unique_ptr<Expr> condition = nullptr;

    if (!this->check(TokenType::SEMICOLON))
        condition.reset(this->unsafe_expression());

    errMessage = "EXPECTED \';\' AFTER LOOP CONDITION";
    this->consume(TokenType::SEMICOLON, errMessage);


    std::unique_ptr<Expr> increment = nullptr;

    if (!this->check(TokenType::RIGHT_PAREN))
        increment.reset(this->unsafe_expression());

    errMessage = "EXPECTED \')\' AFTER FOR CLAUSES";
    this->consume(TokenType::RIGHT_PAREN, errMessage);

    errMessage = "EXPECTED \'DO\' AFTER FOR HEADER";
    this->consume(TokenType::DO, errMessage);


    std::unique_ptr<Stmt> body(this->unsafe_statement()); 

    if (increment.get()) {

        std::vector<std::unique_ptr<Stmt>> statements;

        statements.emplace_back(std::move(body));
        statements.emplace_back(std::make_unique<Expression>(std::move(increment)));

        body = std::make_unique<Block>(std::move(statements));
    }

    if (!condition.get())
        condition = std::make_unique<Literal>(true);

    body = std::make_unique<While>(std::move(condition), std::move(body));


    if (initialiser.get()) {

        std::vector<std::unique_ptr<Stmt>> statements;

        statements.emplace_back(std::move(initialiser));
        statements.emplace_back(std::move(body));

        body = std::make_unique<Block>(std::move(statements));
    }

    return body.release();
}


Stmt* Parser::unsafe_ifStatement(void) { // free memory later

    std::string errMessage = "EXPECTED \'(\' AFTER \'IF\'";
    this->consume(TokenType::LEFT_PAREN, errMessage);

    std::unique_ptr<Expr> condition(this->unsafe_expression());
    errMessage = "EXPECTED \')\' AFTER IF CONDITION";

    this->consume(TokenType::RIGHT_PAREN, errMessage);
    errMessage = "EXPECTED \'THEN\' AFTER IF CONDITION";

    this->consume(TokenType::THEN, errMessage);
    errMessage = "EXPECTED 'DO' AFTER IF CONDITION";

    this->consume(TokenType::DO, errMessage);

    std::unique_ptr<Stmt> thenBranch(this->unsafe_statement());
    std::unique_ptr<Stmt> elseBranch;

    if (this->match({TokenType::ELSE})) {

        errMessage = "EXPECTED \'DO\' AFTER \'ELSE\'";
        this->consume(TokenType::DO, errMessage);

        elseBranch.reset(this->unsafe_statement());
    }

    return new If(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}


Stmt* Parser::unsafe_printStatement(void) { // free memory later

    std::unique_ptr<Expr> val(this->unsafe_expression());

    std::string errMessage = "EXPECTED \';\' AFTER VALUE";
    this->consume(TokenType::SEMICOLON, errMessage);

    return new Print(std::move(val));
}


Stmt* Parser::unsafe_printlnStatement(void) { // free memory later

    std::unique_ptr<Expr> val(this->unsafe_expression());

    std::string errMessage = "EXPECTED \';\' AFTER VALUE";
    this->consume(TokenType::SEMICOLON, errMessage);

    return new Println(std::move(val));
}


Stmt* Parser::unsafe_expressionStatement(void) { // free memory later

    std::unique_ptr<Expr> expr(this->unsafe_expression());

    std::string errMessage = "EXPECTED \';\' AFTER EXPRESSION";
    this->consume(TokenType::SEMICOLON, errMessage);

    return new Expression(std::move(expr));
}

Stmt* Parser::unsafe_declaration(void) { // free memory later

    try {

        if (this->match({TokenType::DEF}))
            return this->unsafe_function("function");

        if (this->match({TokenType::VAR}))
            return this->unsafe_varDeclaration();

        return this->unsafe_statement();
    
    } catch (const Parser::ParseError&) {

        this->synchronise();

        return nullptr;
    }
}


Stmt* Parser::unsafe_function(const std::string& kind) {

    std::string errMessage = "EXPECTED " + kind + " NAME";
    Token name = this->consume(TokenType::IDENTIFIER, errMessage);

    errMessage = "EXPECTED \'(\' AFTER FUNCTION NAME";
    this->consume(TokenType::LEFT_PAREN, errMessage);

    std::vector<Token> params;

    if (!this->check(TokenType::RIGHT_PAREN)) {

        do {

            if (params.size() >= 255) {

                std::string errMessage = "CANNOT PASS MORE THAN 255 PARAMETERS"; 
                cscript::error(this->peek(), errMessage);
            }

            std::string errMessage = "EXPECTED PARAMETER NAME";

            params.emplace_back(this->consume(TokenType::IDENTIFIER, errMessage));

        } while (this->match({TokenType::COMMA}));
    }

    errMessage = "EXPECTED \')\' AFTER PARAMETERS";
    this->consume(TokenType::RIGHT_PAREN, errMessage);

    errMessage = "EXPECTED \'AS\' FUNCTION HEADER TO BODY LINKER";
    this->consume(TokenType::AS, errMessage);
    
    errMessage = "EXPECTED \'DO\' BLOCK INITIALISER BEFORE" + kind + "BODY";
    this->consume(TokenType::DO, errMessage);

    errMessage = "EXPECTED \'{\' BEFORE " + kind + " BODY";
    this->consume(TokenType::LEFT_BRACE, errMessage);

    std::vector<std::unique_ptr<Stmt>> body = this->unsafe_block();

    return new Function(name, std::move(params), std::move(body));
}


Stmt* Parser::unsafe_varDeclaration(void) {

    std::string errMessage = "EXPECTED VARIABLE NAME";
    Token name = this->consume(TokenType::IDENTIFIER, errMessage);

    std::unique_ptr<Expr> initialiser;

    if (this->match({TokenType::ASSIGN}))
        initialiser.reset(this->unsafe_expression());

    errMessage = "EXPECTED \';\' AFTER VARIABLE DECLARATION";
    this->consume(TokenType::SEMICOLON, errMessage);

    return new Var(name, std::move(initialiser));
}


Stmt* Parser::unsafe_whileStatement(void) {

    std::string errMessage = "EXPECETED \'(\' AFTER \'WHILE\'";
    this->consume(TokenType::LEFT_PAREN, errMessage);

    std::unique_ptr<Expr> condition(this->unsafe_expression());

    errMessage = "EXPECTED \')\' AFTER WHILE CONDITION";
    this->consume(TokenType::RIGHT_PAREN, errMessage);

    errMessage = "EXPECTED 'DO' AFTER WHILE CONDITION";
    this->consume(TokenType::DO, errMessage);

    std::unique_ptr<Stmt> body(this->unsafe_statement());

    return new While(std::move(condition), std::move(body));
}


std::vector<std::unique_ptr<Stmt>> Parser::unsafe_block(void) {

    std::vector<std::unique_ptr<Stmt>> statements;
    
    while (!this->check(TokenType::RIGHT_BRACE) && !this->isAtEnd())
        statements.emplace_back(this->unsafe_declaration());

    std::string errMessage = "EXPECTED \'}\' AFTER BLOCK";

    this->consume(TokenType::RIGHT_BRACE, errMessage);

    return statements;
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
    }

    return;
}
























