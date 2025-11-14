#include "../funcs/Callable.hpp"
#include "./Interpreter.hpp"
#include "../lexer/Expr.hpp"
#include "../tokeniser/Token.hpp"
#include "../main/cscript.hpp"
#include "../superclass/super.hpp"
#include "../lexer/Stmt.hpp"
#include "../environment/Environment.hpp"
#include "../funcs/Function.hpp"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sysexits.h>
#include <format>
#include <chrono>
#include <vector>


namespace {

    class ClockCallable : public Callable {

        private:



        public:

            int arity(void) const override {

                return 0;
            }

            super::object call(Interpreter& interpreter, std::vector<super::object>& args) override {

                auto now = std::chrono::system_clock::now();

                auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

                return super::object(static_cast<double>(millis) / 1000.0);
            }

            std::string to_string(void) const override {

                return "<native fn>";
            }

            ~ClockCallable() = default;
    };
}


Interpreter::Interpreter() {

    this->global = std::make_unique<Environment>();
    this->environment = this->global.get();

    std::string lexeme = "clock";
    super::object nil(nullptr);

    this->global->define(

        Token (

            TokenType::IDENTIFIER,
            lexeme,
            nil,
            0

        ), super::object(std::make_shared<ClockCallable>()));
}


super::object Interpreter::visitGroupingExpr(const Grouping& expr) {

    return this->evaluate(*expr.expr.get());
}


super::object Interpreter::evaluate(Expr& expr) {

    return expr.accept(*this);
}


super::object Interpreter::visitUnaryExpr(const Unary& expr) {

    super::object right = this->evaluate(*expr.right.get());

    switch (expr.oprtor.type) {

        case TokenType::MINUS:

            this->checkNumberOperand(expr.oprtor, right);

            return -right.as_double();

            break;

        
        case TokenType::BANG:

            return !this->isTruthy(right);


        default:
            break;
    }

    return super::object();
}


super::object Interpreter::visitBinaryExpr(const Binary& expr) {

    super::object left = this->evaluate(*expr.left.get());
    super::object right = this->evaluate(*expr.right.get());

    switch (expr.oprtor.type) {

        case TokenType::MINUS:

            this->checkNumberOperands(expr.oprtor, left, right);

            return left.as_double() - right.as_double();

            break;


        case TokenType::SLASH:

            this->checkNumberOperands(expr.oprtor, left, right);

            if (!static_cast<int>(right.as_double()))
                throw Interpreter::RuntimeError(expr.oprtor, "Interpreter: DIVISION BY ZERO");

            return left.as_double() / right.as_double();

            break;


        case TokenType::STAR:

            this->checkNumberOperands(expr.oprtor, left, right);

            return left.as_double() * right.as_double();

            break;


        case TokenType::PLUS:

            if (left.is_double() && right.is_double())
                return left.as_double() + right.as_double();

            if (left.is_string() || right.is_string())
                return left.to_string() + right.to_string();

            throw Interpreter::RuntimeError(expr.oprtor, "Interpreter: OPERANDS MUST BE BOTH NUMBERS OR ATLEAST ONE STRING");

            break;


        case TokenType::POWER:

            this->checkNumberOperands(expr.oprtor, left, right);

            return std::pow(left.as_double(), right.as_double());

            break;


        case TokenType::MOD:

            this->checkNumberOperands(expr.oprtor, left, right);

            if (!static_cast<int>(right.as_double()))
                throw Interpreter::RuntimeError(expr.oprtor, "Interpreter: DIVISION BY ZERO");

            return std::fmod(left.as_double(), right.as_double());

            break;


        case TokenType::GREATER:

            this->checkNumberOperands(expr.oprtor, left, right);

            return left.as_double() > right.as_double();

            break;


        case TokenType::GREATER_EQUAL:

            this->checkNumberOperands(expr.oprtor, left, right);

            return left.as_double() >= right.as_double();

            break;


        case TokenType::LESS:

            this->checkNumberOperands(expr.oprtor, left, right);
            
            return left.as_double() < right.as_double();

            break;


        case TokenType::LESS_EQUAL:

            this->checkNumberOperands(expr.oprtor, left, right);

            return left.as_double() <= right.as_double();

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

    return super::object();
}


super::object Interpreter::visitCallExpr(const Call& expr) {

    super::object callee = this->evaluate(*expr.callee.get());

    std::vector<super::object> args;

    for (int i = 0; i < expr.args.size(); i++)
        args.emplace_back(this->evaluate(*expr.args[i].get()));

    if (!callee.is_callable())
        throw Interpreter::RuntimeError(expr.paren, "CAN CALL ONLY FUNCTIONS AND CLASSES");

    std::shared_ptr<Callable> function = callee.as_callable();

    if (args.size() != function->arity())
        throw Interpreter::RuntimeError(expr.paren, "EXPECTED " + std::to_string(function->arity()) + " ARGUMENTS BUT GOT " + std::to_string(args.size()));

    return function->call(*this, args);
}


super::object Interpreter::visitLiteralExpr(const Literal& expr) {

    return expr.value;
}


super::object Interpreter::visitLogicalExpr(const Logical& expr) {

    super::object left = this->evaluate(*expr.left.get());

    if (expr.operatr.type == TokenType::OR) {

        if (this->isTruthy(left))
            return left;

    } else if (!this->isTruthy(left))
            return left;

    return this->evaluate(*expr.right.get());
}


super::object Interpreter::visitExpressionStmt(const Expression& stmt) {

    super::object val = this->evaluate(*stmt.expr.get());

    if (this->repl)
        std::cout << val.to_string() << std::endl;

    return nullptr;
}


super::object Interpreter::visitFunctionStmt(const Function& stmt) {

    std::shared_ptr<Callable> function = std::make_shared<CallableFunction>(&stmt);

    environment->define(stmt.name, function);

    return nullptr;
}


super::object Interpreter::visitIfStmt(const If& stmt) {

    if (this->isTruthy(this->evaluate(*stmt.condition.get())))
        this->execute(stmt.thenBranch.get());

    else if (stmt.elseBranch.get())
        this->execute(stmt.elseBranch.get());

    return nullptr;
}


super::object Interpreter::visitPrintStmt(const Print& stmt) {

    super::object val = this->evaluate(*stmt.expr.get());

    std::cout << val.to_string();

    return nullptr;
}


super::object Interpreter::visitPrintlnStmt(const Println& stmt) {

    super::object val = this->evaluate(*stmt.expr.get());

    std::cout << val.to_string() << std::endl;

    return nullptr;
}


super::object Interpreter::visitVarStmt(const Var& stmt) {

    super::object val = super::uninitialised_t{};

    if (stmt.initialiser.get())
        val = this->evaluate(*stmt.initialiser.get());

    this->environment->define(stmt.name, val);

    return nullptr;
}


super::object Interpreter::visitWhileStmt(const While& stmt) {

    while (this->isTruthy(this->evaluate(*stmt.condition.get())))
        this->execute(stmt.body.get());

    return nullptr;
}


super::object Interpreter::visitVariableExpr(const Variable& expr) {

    return this->environment->get(expr.name);
}


super::object Interpreter::visitAssignExpr(const Assign& expr) {

    super::object val = this->evaluate(*expr.value.get());

    this->environment->assign(expr.name, val);

    return val;
}


super::object Interpreter::visitBlockStmt(const Block& stmt) {

    this->executeBlock(stmt.statements, new Environment(this->environment));

    return nullptr;
}


void Interpreter::executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, Environment* environment) {

    Environment* previous = this->environment;

    try {

        this->environment = environment;

        for (auto& statement : statements) 
            this->execute(statement.get());
    
    } catch (std::exception&) {

        this->environment = previous;
        
        delete environment;

        throw;
    }

    this->environment = previous;

    delete environment;

    return;
}


bool Interpreter::isTruthy(super::object obj) {

    if (obj.is_null() || obj.is_uninitialised())
        return false;

    if (obj.is_bool())
        return obj.as_bool();

    if (obj.is_double())
        return static_cast<bool>(obj.as_double());

    return true;
}



void Interpreter::interpret(std::vector<Stmt*>& statements, bool repl) {

    this->repl = repl;

    try {

        for (auto& statement : statements)
            this->execute(statement);
    
    } catch (const Interpreter::RuntimeError& e) {

        cscript::runtimeError(e);
    }
}


void Interpreter::execute(Stmt* stmt) {

    stmt->accept(*this);
}


Interpreter::RuntimeError::RuntimeError(const Token& token, const std::string& errMessage)
    : std::runtime_error(errMessage),
      token(token) {}


void Interpreter::checkNumberOperand(const Token& operatr, const super::object operand) {

    if (operand.is_double())
        return;

    throw Interpreter::RuntimeError(operatr, "Interpreter: OPERAND MUST BE A NUMBER");
}


void Interpreter::checkNumberOperands(const Token& operatr, const super::object left, const super::object right) {

    if (left.is_double() && right.is_double())
        return;

    throw Interpreter::RuntimeError(operatr, "Interpreter: OPERANDS MUST BE NUMBERS");
}















