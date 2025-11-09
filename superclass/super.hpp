#pragma once

#include <cstddef>
#include <variant>
#include <string>


using object = std::variant<

    bool,
    double,
    std::string,
    std::nullptr_t
>;




