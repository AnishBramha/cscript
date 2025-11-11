#include "./Environment.hpp"
#include "../superclass/super.hpp"
#include "../tokeniser/Token.hpp"
#include "../interpreter/Interpreter.hpp"
#include <stdexcept>
#include <unordered_map>


void Environment::define(const std::string& name, super::object value) {

    this->values.emplace(std::pair{name, value});
}


super::object Environment::get(Token name) {

    if (this->values.find(name.lexeme) != this->values.end())
        return this->values.at(name.lexeme);

    throw Interpreter::RuntimeError(name, "UNDEFINED VARIABLE \'" + name.lexeme + "\'");
}





