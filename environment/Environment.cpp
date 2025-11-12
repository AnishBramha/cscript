#include "./Environment.hpp"
#include "../superclass/super.hpp"
#include "../tokeniser/Token.hpp"
#include "../interpreter/Interpreter.hpp"
#include "../main/cscript.hpp"
#include <stdexcept>
#include <unordered_map>


Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(Environment* enclosing)
    : enclosing(enclosing) {}


void Environment::define(const Token& name, super::object val) {

    if (this->values.find(name.lexeme) == this->values.end())
        this->values.emplace(std::pair{name.lexeme, val});

    else
        throw Interpreter::RuntimeError(name, "CANNOT REDECLARE VARIABLE \'" + name.lexeme + "\'");

    return;
}


super::object Environment::get(const Token& name) {

    if (this->values.find(name.lexeme) != this->values.end()) {

        super::object val = this->values.at(name.lexeme);

        if (val.is_uninitialised())
            throw Interpreter::RuntimeError(name, "UNINITIALISED VARIABLE \'" + name.lexeme + "\'");

        return val;
    }

    if (this->enclosing)
        return this->enclosing->get(name);

    throw Interpreter::RuntimeError(name, "UNDEFINED VARIABLE \'" + name.lexeme + "\'");
}


void Environment::assign(const Token& name, super::object val) {

    if (this->values.find(name.lexeme) != this->values.end()) {

        this->values.at(name.lexeme) = val;

        return;
    }

    if (this->enclosing) {

        this->enclosing->assign(name, val);

        return;
    }

    throw Interpreter::RuntimeError(name, "UNDEFINED VARIABLE \'" + name.lexeme + "\'");
}







