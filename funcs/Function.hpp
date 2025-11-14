#pragma once

#include "Callable.hpp"
#include "../lexer/Stmt.hpp"

class CallableFunction : public Callable {

    private:

        const Function* declaration;

    public:
      
        CallableFunction(const Function* declaration);

        int arity(void) const override;
        super::object call(Interpreter& interpreter, std::vector<super::object>& args) override;
        std::string to_string(void) const override;

        ~CallableFunction() = default;
};






