#include "./Resolver.hpp"
#include "../lexer/Stmt.hpp"
#include <cstddef>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include "../main/cscript.hpp"


Resolver::Resolver(Interpreter& interpreter) : interpreter(interpreter) {}


super::object Resolver::visitBlockStmt(const Block& stmt) {

    this->beginScope();
    this->resolve(stmt.statements);
    this->endScope();

    return nullptr;
}


super::object Resolver::visitVarStmt(const Var& stmt) {

    this->declare(stmt.name);
    
    if (stmt.initialiser.get())
        this->resolve(stmt.initialiser.get());

    this->define(stmt.name);

    return nullptr;
}


super::object Resolver::visitVariableExpr(const Variable& expr) {

    if (!this->scopes.empty() && this->scopes.top().find(expr.name.lexeme) != this->scopes.top().end() && !this->scopes.top().find(expr.name.lexeme)->second) {

        std::string errMessage = "CANNOT READ LOCAL VARIABLE TO ITS OWN INITIALISER";
        cscript::error(expr.name, errMessage);
    }

    this->resolveLocal(const_cast<Variable*>(&expr), expr.name);

    return nullptr;
}


super::object Resolver::visitAssignExpr(const Assign& expr) {

    this->resolve(expr.value.get());
    this->resolveLocal(const_cast<Assign*>(&expr), expr.name);

    return nullptr;
}

super::object Resolver::visitFunctionStmt(const Function& stmt) {

    this->declare(stmt.name);
    this->define(stmt.name);

    this->resolveFunction(const_cast<Function*>(&stmt), Resolver::FunctionType::FUNCTION);

    return nullptr;
}

super::object Resolver::visitExpressionStmt(const Expression& stmt) {
    
    this->resolve(stmt.expr.get());

    return nullptr;
}

super::object Resolver::visitIfStmt(const If& stmt) {

    this->resolve(stmt.condition.get());
    this->resolve(stmt.thenBranch.get());

    if (stmt.elseBranch.get())
        this->resolve(stmt.elseBranch.get());

    return nullptr;
}


super::object Resolver::visitPrintStmt(const Print& stmt) {

    this->resolve(stmt.expr.get());

    return nullptr;
}


super::object Resolver::visitPrintlnStmt(const Println& stmt) {

    this->resolve(stmt.expr.get());

    return nullptr;
}


super::object Resolver::visitReturnStmt(const Return& stmt) {

    if (this->currentFunction == Resolver::FunctionType::NONE) {

        std::string errMessage = "STRAY RETURN STATEMENT IN GLOBAL SCOPE";
        cscript::error(stmt.keyword, errMessage);
    }

    if (stmt.value.get()) {

        if (this->currentFunction == FunctionType::INITIALISER) {

            std::string errMessage = "ILLEGAL VALUE JUMP FROM CONSTRUCTOR";
            cscript::error(stmt.keyword, errMessage);
        }

        this->resolve(stmt.value.get());
    }

    return nullptr;
}


super::object Resolver::visitClassStmt(const Class& stmt) {

    ClassType enclosingClass = this->currentClass;
    this->currentClass = ClassType::CLASS;

    this->declare(stmt.name);
    this->define(stmt.name);

    this->beginScope();
    this->scopes.top().emplace(std::make_pair("this", true));

    for (const std::unique_ptr<Stmt>& method : stmt.methods) {

        FunctionType declaration = FunctionType::METHOD;

        if (dynamic_cast<Function*>(method.get())->name.lexeme == "init")
            declaration = FunctionType::INITIALISER;

        this->resolveFunction(dynamic_cast<Function*>(method.get()), declaration);
    }

    this->endScope();

    this->currentClass = enclosingClass;

    return nullptr;
}


super::object Resolver::visitWhileStmt(const While& stmt) {

    this->resolve(stmt.condition.get());
    this->resolve(stmt.body.get());

    return nullptr;
}


super::object Resolver::visitBinaryExpr(const Binary& expr) {

    this->resolve(expr.left.get());
    this->resolve(expr.right.get());

    return nullptr;
}


super::object Resolver::visitCallExpr(const Call& expr) {
    
    this->resolve(expr.callee.get());

    for (const std::unique_ptr<Expr>& arg : expr.args)
        this->resolve(arg.get());

    return nullptr;
}


super::object Resolver::visitGetExpr(const Get& expr) {

    this->resolve(expr.obj.get());

    return nullptr;
}


super::object Resolver::visitGroupingExpr(const Grouping& expr) {

    this->resolve(expr.expr.get());

    return nullptr;
}


super::object Resolver::visitLiteralExpr(const Literal&) {

    return nullptr;
}


super::object Resolver::visitLogicalExpr(const Logical& expr) {

    this->resolve(expr.left.get());
    this->resolve(expr.right.get());

    return nullptr;
}

super::object Resolver::visitSetExpr(const Set& expr) {

    this->resolve(expr.val.get());
    this->resolve(expr.obj.get());

    return nullptr;
}

super::object Resolver::visitThisExpr(const This& expr) {

    if (this->currentClass == ClassType::NONE) {

        std::string errMessage = "ILLEGAL REFERENCE TO \'this\' OUTSIDE A CLASS";
        cscript::error(expr.keyword, errMessage);

        return nullptr;
    }

    this->resolveLocal(const_cast<This*>(&expr), expr.keyword);

    return nullptr;
}


super::object Resolver::visitUnaryExpr(const Unary& expr) {

    this->resolve(expr.right.get());

    return nullptr;
}


void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& statements) {

    for (const auto& statement : statements)
        this->resolve(statement.get());

    return;
}


void Resolver::resolve(const std::vector<Stmt*>& statements) {

    for (const auto& statement : statements)
        this->resolve(statement);

    return;
}


void Resolver::resolve(Stmt* stmt) {

    stmt->accept(*this);

    return;
}


void Resolver::resolve(Expr* expr) {

    expr->accept(*this);

    return;
}

void Resolver::resolveLocal(Expr* expr, const Token& name) {

    std::stack<std::unordered_map<std::string, bool>> temp;

    if (!this->scopes.empty()) {

        int size = static_cast<int>(this->scopes.size());

        for (int i = size - 1; i >= 0; i--) {

            if (this->scopes.top().find(name.lexeme) != this->scopes.top().end()) {

                this->interpreter.resolve(expr, static_cast<int>(size) - 1 - i);
            
                goto restore;
            }

            temp.emplace(this->scopes.top());
            this->scopes.pop();
        }
    }

    restore:

        while (!temp.empty()) {

            this->scopes.emplace(temp.top());
            temp.pop();
        }

    return;
}


void Resolver::resolveFunction(Function* function, Resolver::FunctionType type) {

    Resolver::FunctionType enclosingFunction = this->currentFunction;
    this->currentFunction = type;

    this->beginScope();

    for (const Token& param : function->params) {

        this->declare(param);
        this->define(param);
    }

    this->resolve(function->body);
    this->endScope();
    this->currentFunction = enclosingFunction;

    return;
}


void Resolver::beginScope(void) {

    this->scopes.emplace();
    
    return;
}


void Resolver::endScope(void) {

    this->scopes.pop();

    return;
}


void Resolver::declare(const Token& name) {

    if (this->scopes.empty())
        return;

    if (this->scopes.top().find(name.lexeme) != this->scopes.top().end()) {

        std::string errMessage = "CANNOT REDECLARE VARIABLE IN THE SAME SCOPE";
        cscript::error(name, errMessage);
    }

    this->scopes.top().emplace(std::make_pair(name.lexeme, false));

    return;
}


void Resolver::define(const Token& name) {

    if (this->scopes.empty())
        return;

    this->scopes.top()[name.lexeme] =  true;

    return;
}




















