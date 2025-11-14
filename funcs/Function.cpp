#include "./Function.hpp"
#include "../superclass/super.hpp"
#include <memory>
#include <utility>
#include <vector>


CallableFunction::CallableFunction(const Function* declaration)
    : declaration(declaration) {}
    

super::object CallableFunction::call(Interpreter& interpreter, std::vector<super::object>& args) {

    std::unique_ptr<Environment> environment = std::make_unique<Environment>(interpreter.global.get());

    for (int i = 0; i < declaration->params.size(); i++)
        environment->define(declaration->params.at(i), args.at(i));

    interpreter.executeBlock(declaration->body, environment.release());

    return nullptr;
}


int CallableFunction::arity(void) const {

    return this->declaration->params.size();
}


std::string CallableFunction::to_string(void) const {

    return "<fn " + declaration->name.lexeme + ">";
};








