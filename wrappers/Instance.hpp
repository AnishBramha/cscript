#pragma once

#include "./Class.hpp"
#include <string>
#include <unordered_map>


class Instance {

    private:

        CallableClass _class;
        std::unordered_map<std::string, super::object> fields;

    
    public:

        Instance(CallableClass& _class);

        std::string to_string(void) const;
        super::object get(const Token& name) const;
        void set(const Token& name, super::object val);

        ~Instance() = default;
};






