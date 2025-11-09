#include "./Interpreter.hpp"
#include "../syntax/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../main/cscript.hpp"
#include "../superclass/super.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sysexits.h>
#include <type_traits>
#include <variant>
#include <format>


object Interpreter::visitGroupingExpr(const Grouping& expr) {

    return this->evaluate(*expr.expr.get());
}


object Interpreter::evaluate(Expr& expr) {

    return expr.accept(*this);
}


object Interpreter::visitUnaryExpr(const Unary& expr) {

    object right = this->evaluate(*expr.right.get());

    switch (expr.oprtor.type) {

        case TokenType::MINUS:

            this->checkNumberOperand(expr.oprtor, right);

            return -(*std::get_if<double>(&right));

            break;

        
        case TokenType::BANG:

            return !this->isTruthy(right);


        default:
            break;
    }

    return nullptr;
}


object Interpreter::visitBinaryExpr(const Binary& expr) {

    object left = this->evaluate(*expr.left.get());
    object right = this->evaluate(*expr.right.get());

    auto* double_left_ptr = std::get_if<double>(&left);
    auto* double_right_ptr = std::get_if<double>(&right);

    auto* str_left_ptr = std::get_if<std::string>(&left);
    auto* str_right_ptr = std::get_if<std::string>(&right);


    switch (expr.oprtor.type) {

        case TokenType::MINUS:

            this->checkNumberOperands(expr.oprtor, left, right);

            return *double_left_ptr - *double_right_ptr;

            break;


        case TokenType::SLASH:

            this->checkNumberOperands(expr.oprtor, left, right);

            if (!(*double_right_ptr))
                throw Interpreter::RuntimeError(expr.oprtor, "Interpreter: DIVISION BY ZERO");

            return *double_left_ptr / *double_right_ptr;

            break;


        case TokenType::STAR:

            this->checkNumberOperands(expr.oprtor, left, right);

            return *double_left_ptr * *double_right_ptr;

            break;


        case TokenType::PLUS:

            if (double_left_ptr && double_right_ptr)
                return *double_left_ptr + *double_right_ptr;

            if (str_left_ptr && str_right_ptr)
                return *str_left_ptr + *str_right_ptr;

            if (str_left_ptr && double_right_ptr)
                return *str_left_ptr + std::to_string(*double_right_ptr);

            if (double_left_ptr && str_right_ptr)
                return std::to_string(*double_left_ptr) + *str_right_ptr;

            throw Interpreter::RuntimeError(expr.oprtor, "Interpreter: OPERANDS MUST BE NUMBERS OR STRINGS");

            break;


        case TokenType::GREATER:

            this->checkNumberOperands(expr.oprtor, left, right);

            return *double_left_ptr > *double_right_ptr;

            break;


        case TokenType::GREATER_EQUAL:

            this->checkNumberOperands(expr.oprtor, left, right);

            return *double_left_ptr >= *double_right_ptr;

            break;


        case TokenType::LESS:

            this->checkNumberOperands(expr.oprtor, left, right);
            
            return *double_left_ptr < *double_right_ptr;

            break;


        case TokenType::LESS_EQUAL:

            this->checkNumberOperands(expr.oprtor, left, right);

            return *double_left_ptr <= *double_right_ptr;

            break;


        case TokenType::NOT_EQUAL:

            return left != right;

            break;

        
        case TokenType::EQUAL:

            return left == right;

            break;


        default:
            break;
    }

    return nullptr;
}


object Interpreter::visitLiteralExpr(const Literal& expr) {

    return expr.value;
}


bool Interpreter::isTruthy(object obj) {

    return std::visit(

        [&](auto&& arg) -> bool {

            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, nullptr_t>)
                return false;

            else if constexpr (std::is_same_v<T, bool>)
                return arg;

            return true;

        }, obj
    );
}


void Interpreter::interpret(Expr& expr) {

    try {

        object val = this->evaluate(expr);

        std::cout << this->objectToString(val) << std::endl;

    } catch (const Interpreter::RuntimeError& e) {

        cscript::runtimeError(e);
    }

    return;
}


Interpreter::RuntimeError::RuntimeError(const Token& token, const std::string& errMessage)
    : std::runtime_error(errMessage),
      token(token) {}


void Interpreter::checkNumberOperand(const Token& operatr, const object operand) {

    if (std::holds_alternative<double>(operand))
        return;

    throw Interpreter::RuntimeError(operatr, "Interpreter: OPERAND MUST BE A NUMBER");
}


void Interpreter::checkNumberOperands(const Token& operatr, const object left, const object right) {

    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        return;

    throw Interpreter::RuntimeError(operatr, "Interpreter: OPERANDS MUST BE NUMBERS");
}


std::string Interpreter::objectToString(object obj) {

    if (std::holds_alternative<std::nullptr_t>(obj))
        return "nil";

    if (std::holds_alternative<double>(obj))
        return std::format("{}", *std::get_if<double>(&obj));

    if (std::holds_alternative<bool>(obj))
        return *std::get_if<bool>(&obj) ? "true" : "false";

    if (std::holds_alternative<std::string>(obj))
        return *std::get_if<std::string>(&obj);

    return "Interpreter: UNEXPECTED CASE";
}












