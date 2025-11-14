#include "./Scanner.hpp"
#include "../tokeniser/Token.hpp"
#include "../main/cscript.hpp"
#include <cctype>
#include <cstddef>
#include <string>
#include <vector>
#include "../superclass/super.hpp"


Scanner::Scanner(std::string& source) : source(source) {}

std::vector<Token*> Scanner::scanTokens(void) {

    while (!this->isAtEnd()) {

        this->start = this->current;
        
        this->scanToken();
    }

    std::string lexeme = "";
    super::object literal = nullptr;

    this->tokens.emplace_back(new Token(TokenType::END_OF_FILE, lexeme, literal, this->line));

    return this->tokens;
}


char Scanner::peek(void) {

    if (this->isAtEnd())
        return '\0';

    return this->source.at(this->current);
}


char Scanner::peekNext(void) {

    if (this->current + 1 >= this->source.length())
        return '\0';

    return this->source.at(this->current + 1);
}


void Scanner::number(void) {

    while (std::isdigit(this->peek()))
        this->advance();

    // look for fractional part
    if (this->peek() == '.' && std::isdigit(this->peekNext())) {

        this->advance();

        while (std::isdigit(this->peek()))
            this->advance();
    }

    super::object number = std::stod(this->source.substr(this->start, this->current - this->start));

    this->addToken(TokenType::NUMBER, number);

    return;
}


void Scanner::identifier(void) {

    while (std::isalnum(this->peek()) || this->peek() == '_')
        this->advance();

    std::string text = this->source.substr(this->start, this->current - this->start);
    auto it = Scanner::keywords.find(text);

    TokenType type;

    if (it == Scanner::keywords.end())
        type = TokenType::IDENTIFIER;

    else
        type = it->second;

    this->addToken(type);

    return;
}


void Scanner::scanToken(void) {

    char c = this->advance();

    switch (c) {

        case '(': this->addToken(TokenType::LEFT_PAREN);  break;
        case ')': this->addToken(TokenType::RIGHT_PAREN); break;
        case '{': this->addToken(TokenType::LEFT_BRACE);  break;
        case '}': this->addToken(TokenType::RIGHT_BRACE); break;
        case ',': this->addToken(TokenType::COMMA);       break;
        case '.': this->addToken(TokenType::DOT);         break;
        case '-': this->addToken(TokenType::MINUS);       break;
        case '+': this->addToken(TokenType::PLUS);        break;
        case ';': this->addToken(TokenType::SEMICOLON);   break;
        case '*': this->addToken(TokenType::STAR);        break;
        case '!': this->addToken(TokenType::BANG);        break;
        case '=': this->addToken(TokenType::EQUAL);       break;
        case '^': this->addToken(TokenType::POWER);       break;
        case '%': this->addToken(TokenType::MOD);         break;

        case ':':

            if (this->match('='))
                this->addToken(TokenType::ASSIGN);

            break;

        case '<':

            this->addToken(this->match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;

        case '>':

            this->addToken(this->match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);

            break;


        case '/':

            if (match('='))
                this->addToken(TokenType::NOT_EQUAL);

            else if (match('/')) // check if it is a single-line comment
                while (this->peek() != '\n' && !this->isAtEnd()) this->advance();

            else if (match('*')) // multiline comment
                this->multilineComment();

            else
                this->addToken(TokenType::SLASH);

            break;


        // ignore whitespace
        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            
            this->line++;
            break;


        case '\"': this->str(); break;


        default:

            if (std::isdigit(static_cast<unsigned char>(c))) // numbers
                    this->number();

            else if (std::isalpha(static_cast<unsigned char>(c))|| c == '_') // identifiers
                    this->identifier();
                
            else {

                std::string errMessage = std::string("UNEXPECTED CHARACTER \'") + c + "\'";

                cscript::error(this->line, errMessage);
            }

            break;
    }
}


bool Scanner::isAtEnd(void) {

    return this->current >= this->source.length();
}


char Scanner::advance(void) {

    return this->source.at(this->current++);
}


void Scanner::addToken(TokenType type) {

    super::object literal = nullptr;

    this->addToken(type, literal);
}


void Scanner::addToken(TokenType type, super::object& literal) {

    std::string text = this->source.substr(this->start, this->current - this->start);

    this->tokens.emplace_back(new Token(type, text, literal, this->line));
}


bool Scanner::match(char expected) {

    if (this->isAtEnd() || this->source.at(this->current) != expected)
        return false;

    this->current++;

    return true;
}


void Scanner::multilineComment(void) {

    int nestingLevel = 1;

    while (nestingLevel > 0 && !this->isAtEnd()) {

        if (this->peek() == '/' && this->peekNext() == '*') {

            this->advance();
            this->advance();
            nestingLevel++;
        
        } else if (this->peek() == '*' && this->peekNext() == '/') {

            this->advance();
            this->advance();
            nestingLevel--;
        
        } else if (this->peek() == '\n') {

            this->line++;
            this->advance();
        
        } else
            this->advance();
    }

    if (nestingLevel > 0) {

        std::string where = "";
        std::string errMessage = "UNTERMINATED MULTILINE COMMENT"; 

        cscript::report(this->line, where, errMessage);
    }

    return;
}


void Scanner::str(void) {

    while (this->peek() != '\"' && !this->isAtEnd()) {

        if (this->peek() == '\n') // enable multiline string support
            this->line++;

        this->advance();
    }

    if (this->isAtEnd()) {

        std::string where = "";
        std::string errMessage = "UNTERMINATED STRING LITERAL";

        cscript::report(this->line, where, errMessage);

        return;
    }

    this->advance();

    super::object literal = this->source.substr(this->start + 1, this->current - this->start - 2);

    this->addToken(TokenType::STRING, literal);

    return;
}


Scanner::~Scanner() {

    for (auto& token : this->tokens) {

        delete token;
        token = nullptr;
    }
}








