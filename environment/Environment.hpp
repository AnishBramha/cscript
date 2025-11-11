#pragma once


#include <string>
#include <unordered_map>
#include "../superclass/super.hpp"
#include "../tokeniser/Token.hpp"


class Environment {

    private:

        std::unordered_map<std::string, super::object> values;

    public:

        void define(const std::string& name, super::object value);

        super::object get(Token name);
};




