#pragma once

#include "../syntax/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../superclass/super.hpp"
#include <stdexcept>
#include <string>


class Interpreter : public Visitor {

    private:

        object evaluate(Expr& expr); 
        bool isTruthy(object);
        void checkNumberOperand(const Token& operatr, const object operand);
        void checkNumberOperands(const Token& operatr, const object left, const object right);
        std::string objectToString(object);

    public:

        object visitGroupingExpr(const Grouping& expr) override;
        object visitUnaryExpr(const Unary& expr) override;
        object visitBinaryExpr(const Binary& expr) override;
        object visitLiteralExpr(const Literal& expr) override;

        void interpret(Expr& expr);


        class RuntimeError : public std::runtime_error {

            public:

                const Token token;

                RuntimeError(const Token& token, const std::string& errMessage);
        };
};







