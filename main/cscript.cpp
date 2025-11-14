#include "./cscript.hpp"
#include "../lexer/Expr.hpp"
#include "../parser/Parser.hpp"
#include "../tokeniser/Token.hpp"
#include "../scanner/Scanner.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <fstream>
#include <utility>
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


void cscript::runFile(std::filesystem::path& path) {

    std::ifstream fin(path, std::ios::binary);

    fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::vector<char> bytes(
    
        (std::istreambuf_iterator<char>(fin)),
        (std::istreambuf_iterator<char>())
    );

    std::string content(bytes.begin(), bytes.end());

    cscript::run(content, false);

    if (cscript::hadError)
        std::exit(EX_IOERR);

    if (cscript::hadRuntimeError)
        std::exit(EX_TEMPFAIL);

    fin.close();

    return;
}


void cscript::runPrompt(void) {

    std::string line;

    std::cout << std::endl << "cscript REPL - Type in expressions and/or statements" << std::endl << std::endl;
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
            std::cout << std::endl << "cscript REPL - Type in expressions and/or statements" << std::endl << std::endl;
            std::cout << "Type 'quit' or 'exit' or <C-d> to close REPL" << std::endl << std::endl;
            std::cout << "Type 'clear' to clear the screen" << std::endl << std::endl;
            std::cout << std::endl << "<<< ";

            continue;
        }

        std::cout << ">>> ";
    
        cscript::run(line, true);
        cscript::hadError = false;

        std::cout << std::endl << "<<< ";
    }

    std::cout << std::endl << "Shutting Down..." << std::endl << std::endl;

    return;
}


void cscript::run(std::string& source, bool repl) {

    Scanner scanner(source);
    
    std::vector<Token*> tokens = scanner.scanTokens();
    std::vector<Token> tokens_list;

    for (auto& token : tokens)
        tokens_list.emplace_back(*token);

    Parser parser(tokens_list);
    std::vector<Stmt*> statements = parser.unsafe_parse();

    if (cscript::hadError) {

        if (repl) {

            try {

                cscript::hadError = false;
                Parser parser(tokens_list);

                Expr* expr = parser.unsafe_expression();

                if (!cscript::hadError && parser.isAtEnd()) {

                    std::unique_ptr<Expr> _expr(expr);
                    Stmt* print = new Print(std::move(_expr));

                    std::vector<Stmt*> replStatements;
                    replStatements.push_back(print);

                    cscript::interpreter.interpret(replStatements, repl);

                    delete print;
                    print = nullptr;

                    return;
                }
                
            } catch (const Parser::ParseError&) {

            }
        }

        for (auto& statement : statements) {

            delete statement;
            statement = nullptr;
        }

        return;
    }

    cscript::interpreter.interpret(statements, repl);

    for (auto& statement : statements) {

        delete statement;
        statement = nullptr;
    }

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

    std::cerr << e.what();
    std::cerr << "[line " << e.token.line << "]" << std::endl;

    cscript::hadRuntimeError = true;

    return;
}










