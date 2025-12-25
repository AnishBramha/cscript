#include "./Function.hpp"
#include "../superclass/super.hpp"
#include "../tokeniser/Token.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>


CallableFunction::CallableFunction(const Function* declaration, std::shared_ptr<Environment> closure, bool isInitialiser)
    : declaration(declaration),
      closure(closure),
      isInitialiser(isInitialiser) {}
    

super::object CallableFunction::call(Interpreter& interpreter, std::vector<super::object>& args) {

    std::shared_ptr<Environment> environment = std::make_shared<Environment>(this->closure);

    for (size_t i = 0; i < declaration->params.size(); i++)
        environment->define(declaration->params.at(i), args.at(i));

    try {

        interpreter.executeBlock(declaration->body, environment);

    } catch (const CallableFunction::Return& returnVal) {

        if (this->isInitialiser)
            return this->closure->getAt(0, "this");

        return returnVal.val;
    }

    if (isInitialiser)
        return this->closure->getAt(0, "this");

    return nullptr;
}


int CallableFunction::arity(void) const {

    return static_cast<int>(this->declaration->params.size());
}


std::string CallableFunction::to_string(void) const {

    return "<fn " + declaration->name.lexeme + ">";
};


std::shared_ptr<CallableFunction> CallableFunction::bind(std::shared_ptr<Instance> instance) {

    std::string lexeme = "this";
    super::object literal = nullptr;
    const Token _this(TokenType::THIS, lexeme, literal, -1);

    std::shared_ptr<Environment> environment = std::make_shared<Environment>(this->closure);
    environment->define(_this, super::object(instance));

    return std::make_shared<CallableFunction>(declaration, environment, this->isInitialiser);
}


CallableFunction::Return::Return(super::object val)
    : std::runtime_error(""),
      val(val) {}








