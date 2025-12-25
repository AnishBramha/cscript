#include "./Instance.hpp"
#include "../interpreter/Interpreter.hpp"
#include "Class.hpp"
#include "Function.hpp"
#include <memory>
#include <utility>


Instance::Instance(CallableClass& _class) : _class(_class) {}


std::string Instance::to_string(void) const {

    return "<instance " + this->_class.to_string() + ">";
}


super::object Instance::get(const Token& name) const {

    const auto& property = this->fields.find(name.lexeme);

    if (property != this->fields.end())
        return property->second;

    std::shared_ptr<CallableFunction> method = this->_class.findMethod(name.lexeme);

    if (method)
        return super::object(method->bind(*this));

    throw Interpreter::RuntimeError(name, "UNDEFINED PROPERTY \'" + name.lexeme + "\'");
}


void Instance::set(const Token& name, super::object val) {

    this->fields.emplace(std::make_pair(name.lexeme, val));

    return;
}







