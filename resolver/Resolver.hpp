#pragma once

#include "../lexer/Visitor.hpp"
#include "../interpreter/Interpreter.hpp"
#include "../superclass/super.hpp"
#include "../lexer/Stmt.hpp"
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>


class Resolver : public Visitor {

    private:

        Interpreter& interpreter;
        std::stack<std::unordered_map<std::string, bool>> scopes;

        enum class FunctionType {

            NONE,
            FUNCTION,
            INITIALISER,
            METHOD
        };

        enum class ClassType {

            NONE,
            CLASS,
            SUBCLASS
        };

        FunctionType currentFunction = FunctionType::NONE;
        ClassType currentClass = ClassType::NONE;

    public:

        Resolver(Interpreter& interpreter);

        super::object visitVariableExpr(const Variable& expr) override;
        super::object visitAssignExpr(const Assign& expr) override;
        super::object visitBinaryExpr(const Binary& expr) override;
        super::object visitCallExpr(const Call& expr) override;
        super::object visitGetExpr(const Get& expr) override;
        super::object visitGroupingExpr(const Grouping& expr) override;
        super::object visitLiteralExpr(const Literal& expr) override;
        super::object visitLogicalExpr(const Logical& expr) override;
        super::object visitSetExpr(const Set& expr) override;
        super::object visitSuperExpr(const Super& expr) override;
        super::object visitThisExpr(const This& expr) override;
        super::object visitUnaryExpr(const Unary& expr) override;

        super::object visitBlockStmt(const Block& stmt) override;
        super::object visitClassStmt(const Class& stmt) override;
        super::object visitExpressionStmt(const Expression& stmt) override;
        super::object visitFunctionStmt(const Function& stmt) override;
        super::object visitIfStmt(const If& stmt) override;
        super::object visitPrintStmt(const Print& stmt) override;
        super::object visitPrintlnStmt(const Println& stmt) override;
        super::object visitReturnStmt(const Return& stmt) override;
        super::object visitVarStmt(const Var& stmt) override;
        super::object visitWhileStmt(const While& stmt) override;

        void resolve(const std::vector<std::unique_ptr<Stmt>>& statements);
        void resolve(const std::vector<Stmt*>& statements);
        void resolve(Stmt* stmt);
        void resolve(Expr* expr);
        void resolveLocal(Expr* expr, const Token& name);
        void resolveFunction(Function* function, FunctionType type);

        void beginScope(void);
        void endScope(void);

        void declare(const Token& name);
        void define(const Token& name);
};
















