#include "./super.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <format>
#include "../funcs/Callable.hpp"


super::object::object() : obj(nullptr) {}

super::object::object(std::nullptr_t) : obj(nullptr) {}

super::object::object(bool obj) : obj(obj) {}

super::object::object(double obj) : obj(obj) {}

super::object::object(const std::string& obj) : obj(obj) {}

super::object::object(const char* obj) : obj(std::string(obj)) {}

super::object::object(super::uninitialised_t) : obj(super::uninitialised_t{}) {}

super::object::object(std::shared_ptr<Callable> callable) : obj(std::move(callable)) {}


bool super::object::is_uninitialised(void) const {

    return std::holds_alternative<super::uninitialised_t>(this->obj);
}

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

bool super::object::is_callable(void) const {

    return std::holds_alternative<std::shared_ptr<Callable>>(this->obj);
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

std::shared_ptr<Callable> super::object::as_callable(void) const {

    try {

        if (!this->is_callable())
            throw std::bad_variant_access();

        return *std::get_if<std::shared_ptr<Callable>>(&this->obj);

    } catch (const std::bad_variant_access&) {

        std::cerr << "SUPER ERRPR: NOT CALLABLE" << std::endl;
    }
}


std::string super::object::to_string(void) const {

    if (this->is_null() || this->is_uninitialised())
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






