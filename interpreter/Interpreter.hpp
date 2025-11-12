#pragma once

#include "../lexer/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../superclass/super.hpp"
#include "../lexer/Stmt.hpp"
#include "../environment/Environment.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>


class Interpreter : public Visitor {

    private:

        Environment* environment;
        std::unique_ptr<Environment> global;

        super::object evaluate(Expr& expr);
        void execute(Stmt* stmt);
        void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, Environment* environment);

        bool isTruthy(super::object);
        void checkNumberOperand(const Token& operatr, const super::object operand);
        void checkNumberOperands(const Token& operatr, const super::object left, const super::object right);

    public:

        Interpreter();

        super::object visitGroupingExpr(const Grouping& expr) override;
        super::object visitUnaryExpr(const Unary& expr) override;
        super::object visitBinaryExpr(const Binary& expr) override;
        super::object visitLiteralExpr(const Literal& expr) override;

        super::object visitExpressionStmt(const Expression& stmt) override;
        super::object visitPrintStmt(const Print& stmt) override;
        super::object visitVarStmt(const Var& stmt) override;
        super::object visitVariableExpr(const Variable& expr) override;
        super::object visitAssignExpr(const Assign& expr) override;
        super::object visitBlockStmt(const Block& stmt) override;

        void interpret(std::vector<Stmt*>&);

        class RuntimeError : public std::runtime_error {

            public:

                const Token token;

                RuntimeError(const Token& token, const std::string& errMessage);
        };
};







