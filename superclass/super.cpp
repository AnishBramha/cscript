#include "super.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <variant>
#include <format>


super::object::object() : obj(nullptr) {}

super::object::object(std::nullptr_t) : obj(nullptr) {}

super::object::object(bool obj) : obj(obj) {}

super::object::object(double obj) : obj(obj) {}

super::object::object(const std::string& obj) : obj(obj) {}

super::object::object(const char* obj) : obj(std::string(obj)) {}


bool super::object::is_null(void) const {

    return std::holds_alternative<std::nullptr_t>(this->obj);
}

bool super::object::is_bool(void) const {

    return std::holds_alternative<bool>(this->obj);
}

bool super::object::is_double(void) const {

    return std::holds_alternative<double>(this->obj);
}

bool super::object::is_string(void) const {

    return std::holds_alternative<std::string>(this->obj);
}


bool super::object::as_bool(void) const {

    try {

        if (!this->is_bool())
            throw std::bad_variant_access();

        return *std::get_if<bool>(&this->obj);

    } catch (const std::bad_variant_access&) {

        std::cerr << "SUPER ERROR: NOT A BOOL" << std::endl;
    }

    return false;
}

double super::object::as_double(void) const {

    try {

        if (!this->is_double())
            throw std::bad_variant_access();

        return *std::get_if<double>(&this->obj);
    
    } catch (const std::bad_variant_access&) {

        std::cerr << "SUPER ERROR: NOT A DOUBLE" << std::endl;
    }

    return 0.0;
}

std::string super::object::as_string(void) const {

    try {

        if (!this->is_string())
            throw std::bad_variant_access();

        return *std::get_if<std::string>(&this->obj);

    } catch (const std::bad_variant_access&) {

        std::cerr << "SUPER ERROR: NOT A STRING" << std::endl;
    }

    return "";
}


std::string super::object::to_string(void) const {

    if (this->is_null())
        return "nil";

    if (this->is_bool())
        return this->as_bool() ? "true" : "false";

    if (this->is_double())
        return std::format("{}", this->as_double());

    if (this->is_string())
        return this->as_string();

    return "SUPER ERROR: UNEXPECTED OBJECT TYPE";
}


bool super::object::operator==(const super::object& other) const {

    return this->obj == other.obj;
}

bool super::object::operator!=(const super::object& other) const {

    return this->obj != other.obj;
}






