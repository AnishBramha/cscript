#include "./Function.hpp"
#include "../superclass/super.hpp"
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>


CallableFunction::CallableFunction(const Function* declaration, std::shared_ptr<Environment> closure)
    : declaration(declaration),
      closure(closure) {}
    

super::object CallableFunction::call(Interpreter& interpreter, std::vector<super::object>& args) {

    std::shared_ptr<Environment> environment = std::make_shared<Environment>(this->closure.get());

    for (int i = 0; i < declaration->params.size(); i++)
        environment->define(declaration->params.at(i), args.at(i));

    try {

        interpreter.executeBlock(declaration->body, environment);

    } catch (const CallableFunction::Return& returnVal) {

        return returnVal.val;
    }

    return nullptr;
}


int CallableFunction::arity(void) const {

    return this->declaration->params.size();
}


std::string CallableFunction::to_string(void) const {

    return "<fn " + declaration->name.lexeme + ">";
};


CallableFunction::Return::Return(super::object val)
    : std::runtime_error(""),
      val(val) {}








