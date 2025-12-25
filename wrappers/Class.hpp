#pragma once

#include "./Callable.hpp"
#include "../interpreter/Interpreter.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


class CallableFunction;


class CallableClass : public Callable {

    private:

        std::string name;
        std::unordered_map<std::string, std::shared_ptr<CallableFunction>> methods;

    public:

        CallableClass(
                const std::string& name,
                const std::unordered_map<std::string, std::shared_ptr<CallableFunction>>& methods);

        int arity(void) const override;
        super::object call(Interpreter& interpreter, std::vector<super::object>& args) override;
        std::string to_string(void) const override;

        std::shared_ptr<CallableFunction> findMethod(const std::string& name) const;

        ~CallableClass() = default;
};






