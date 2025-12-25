#pragma once

#include "./Callable.hpp"
#include "./Instance.hpp"
#include "../lexer/Stmt.hpp"
#include <memory>
#include <stdexcept>

class CallableFunction : public Callable {

    private:

        const Function* declaration;
        std::shared_ptr<Environment> closure;

    public:
      
        CallableFunction(const Function* declaration, std::shared_ptr<Environment> closure);

        int arity(void) const override;
        super::object call(Interpreter& interpreter, std::vector<super::object>& args) override;
        std::string to_string(void) const override;

        std::shared_ptr<CallableFunction> bind(const Instance& instance);

        ~CallableFunction() = default;


        class Return : public std::runtime_error {

            private:


            public:

                super::object val;

                Return(super::object val);

                ~Return() = default;
        };
};








