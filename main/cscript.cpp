#include "./cscript.hpp"
#include "../lexer/Expr.hpp"
#include "../parser/Parser.hpp"
#include "../tokeniser/Token.hpp"
#include "../scanner/Scanner.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <iterator>
#include <sysexits.h>
#include <unistd.h>


#define CLEAR_SCREEN "\033[2J"
#define CURSOR_JUMP_TOPLEFT "\033[H"


int main(int argc, char** argv) {

    cscript repl;

    if (argc > 2) {

        std::cerr << "USAGE: CSCRIPT" << std::endl;

        std::exit(EX_USAGE);

    } else if (argc == 2) {

        std::filesystem::path path(argv[1]);

        try {

            repl.runFile(path);

        } catch (const std::exception&) {

            std::cerr << "FAILED TO READ FILE / DISK ERROR" << std::endl;

            return EX_IOERR;
        }

    } else
        repl.runPrompt();

    return 0;
}



// testing purposes
//
// int main(void) {
//
//     std::string minus = "-";
//     std::string star = "*";
//
//     object n1 = static_cast<double>(123);
//     Literal* l1 = new Literal(n1);
//     std::unique_ptr<Expr> _l1(l1);
//
//     object n2 = 45.67;
//     Literal* l2 = new Literal(n2);
//     std::unique_ptr<Expr> _l2(l2);
//
//     object null_t = nullptr;
//     Token* t1 = new Token(TokenType::MINUS, minus, null_t, 1);
//     Token* t2 = new Token(TokenType::STAR, star, null_t, 1);
//
//     Unary* u1 = new Unary(*t1, std::move(_l1));
//     std::unique_ptr<Expr> _u1(u1);
//
//     Grouping* g1 = new Grouping(std::move(_l2));
//     std::unique_ptr<Expr> _g1(g1);
//
//     Expr* expr = new Binary(std::move(_u1), *t2, std::move(_g1));
//
//     AstPrinter* astp = new AstPrinter;
//
//     std::cout << astp->print(*expr) << std::endl;
//
//     delete t1; t1 = nullptr;
//     delete t2; t2 = nullptr;
//     delete expr; expr = nullptr;
//     delete astp; astp = nullptr;
//
//     l1 = nullptr; _l1 = nullptr;
//     l2 = nullptr; _l2 = nullptr;
//     _u1 = nullptr; _g1 = nullptr;
//
//     return 0;
// }


void cscript::runFile(std::filesystem::path& path) {

    std::ifstream fin(path, std::ios::binary);

    fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::vector<char> bytes(
    
        (std::istreambuf_iterator<char>(fin)),
        (std::istreambuf_iterator<char>())
    );

    std::string content(bytes.begin(), bytes.end());

    cscript::run(content);

    if (cscript::hadError)
        std::exit(EX_IOERR);

    if (cscript::hadRuntimeError)
        std::exit(EX_TEMPFAIL);

    fin.close();

    return;
}


void cscript::runPrompt(void) {

    std::string line;

    std::cout << std::endl << "cscript REPL - Type in expressions (statements not yet supported)" << std::endl << std::endl;

    std::cout << "Type 'quit' or 'exit' or <C-d> to close REPL" << std::endl << std::endl;

    std::cout << "Type 'clear' to clear the screen" << std::endl << std::endl;

    std::cout << "<<< ";

    while (std::getline(std::cin, line)) {

        if (line == "quit" || line == "exit") {

            std::cout << std::endl << "Shutting Down..." << std::endl << std::endl;

            return;
        }

        if (line == "clear") {

            std::cout << CLEAR_SCREEN << CURSOR_JUMP_TOPLEFT << std::endl;

            std::cout << std::endl << "cscript REPL - Type in expressions (statements not yet supported)" << std::endl << std::endl;

            std::cout << "Type 'quit' or 'exit' or <C-d> to close REPL" << std::endl << std::endl;

            std::cout << "Type 'clear' to clear the screen" << std::endl << std::endl;

            std::cout << std::endl << "<<< ";

            continue;
        }

        std::cout << ">>> ";
    
        cscript::run(line);

        cscript::hadError = false;

        std::cout << std::endl << "<<< ";
    }

    std::cout << std::endl << "Shutting Down..." << std::endl << std::endl;

    return;
}


void cscript::run(std::string& source) {

    Scanner scanner(source);
    
    std::vector<Token*> tokens = scanner.scanTokens();

    std::vector<Token> tokens_list;

    for (auto& token : tokens)
        tokens_list.emplace_back(*token);

    Parser parser(tokens_list);

    Expr* expr = parser.unsafe_parse();

    if (cscript::hadError) {
        
        delete expr;
        expr = nullptr;

        return;
    }

    cscript::interpreter.interpret(*expr);

    delete expr;
    expr = nullptr;

    return;
}


void cscript::error(int line, std::string& message) {

    std::string column = "";

    cscript::report(line, column, message);

    return;
}


void cscript::report(int line, std::string& where, std::string& message) {

    // later implement arrow to show error
    //                 ^
    //                 |
    //                 ~~~~ Like this

    std::cerr << "[Error on line " << line << "] -?- " << where << " -!- " << message << std::endl;

    cscript::hadError = true;
    
    return;
}


void cscript::error(Token token, std::string& errMessage) {

    if (token.type == TokenType::END_OF_FILE) {

        std::string where = " at end"; 

        cscript::report(token.line, where, errMessage);
    
    } else {

        std::string where = " at \'" + token.lexeme + "\'";

        cscript::report(token.line, where, errMessage);
    }

    return;
}


void cscript::runtimeError(const Interpreter::RuntimeError& e) {

    std::cerr << e.what() << std::endl;
    std::cerr << "[line " << e.token.line << "]" << std::endl;

    cscript::hadRuntimeError = true;

    return;
}










