#pragma once

#include "../superclass/super.hpp"
#include <string>
#include <vector>
#include "../interpreter/Interpreter.hpp"


class Callable {

    private:



    public:

        virtual int arity(void) const = 0;

        virtual super::object call(Interpreter& interpreter, std::vector<super::object>& args) = 0;

        virtual std::string to_string(void) const = 0;

        virtual ~Callable() = default;
};








