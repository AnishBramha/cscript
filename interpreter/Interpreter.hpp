#pragma once

#include "../lexer/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../superclass/super.hpp"
#include <stdexcept>
#include <string>


class Interpreter : public Visitor {

    private:

        super::object evaluate(Expr& expr); 
        bool isTruthy(super::object);
        void checkNumberOperand(const Token& operatr, const super::object operand);
        void checkNumberOperands(const Token& operatr, const super::object left, const super::object right);

    public:

        super::object visitGroupingExpr(const Grouping& expr) override;
        super::object visitUnaryExpr(const Unary& expr) override;
        super::object visitBinaryExpr(const Binary& expr) override;
        super::object visitLiteralExpr(const Literal& expr) override;

        void interpret(Expr& expr);


        class RuntimeError : public std::runtime_error {

            public:

                const Token token;

                RuntimeError(const Token& token, const std::string& errMessage);
        };
};







