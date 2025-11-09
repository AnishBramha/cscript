#pragma once

#include "../interpreter/Interpreter.hpp"
#include "../tokeniser/Token.hpp"
#include <cstdlib>
#include <string>
#include <filesystem>
#include <sysexits.h>


class cscript {

    private:

        inline static bool hadError = false;
        inline static bool hadRuntimeError = false;

        inline static Interpreter interpreter = Interpreter();

    public:

        void runFile(std::filesystem::path& path);
        void runPrompt(void);
        void run(std::string& source);
        static void error(int line, std::string& message);
        static void error(Token token, std::string& message);
        static void report(int line, std::string& where, std::string& message);
        static void runtimeError(const Interpreter::RuntimeError&);

        ~cscript() = default;
};




