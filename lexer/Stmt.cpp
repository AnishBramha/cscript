#include "../superclass/super.hpp"

#include "./Stmt.hpp"

using namespace std;
using super::object;


Block::Block(vector<unique_ptr<Stmt>>&& statements) : statements(std::move(statements)) {}

object Block::accept(Visitor& visitor) {

	return visitor.visitBlockStmt(*this);
}


Expression::Expression(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

object Expression::accept(Visitor& visitor) {

	return visitor.visitExpressionStmt(*this);
}


If::If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch) : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

object If::accept(Visitor& visitor) {

	return visitor.visitIfStmt(*this);
}


Print::Print(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

object Print::accept(Visitor& visitor) {

	return visitor.visitPrintStmt(*this);
}


Println::Println(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

object Println::accept(Visitor& visitor) {

	return visitor.visitPrintlnStmt(*this);
}


Var::Var(const Token& name, std::unique_ptr<Expr> initialiser) : name(name), initialiser(std::move(initialiser)) {}

object Var::accept(Visitor& visitor) {

	return visitor.visitVarStmt(*this);
}


While::While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body)) {}

object While::accept(Visitor& visitor) {

	return visitor.visitWhileStmt(*this);
}


