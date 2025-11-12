#pragma once


#include <memory>
#include <string>
#include <unordered_map>
#include "../superclass/super.hpp"
#include "../tokeniser/Token.hpp"


class Environment {

    private:

        std::unordered_map<std::string, super::object> values;

        Environment* enclosing;

    public:

        Environment();
        Environment(Environment* enclosing);

        void define(const Token& name, super::object value);
        void assign(const Token& name, super::object val);

        super::object get(const Token& name);

        ~Environment() = default;
};






