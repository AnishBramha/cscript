#include "../superclass/super.hpp"

#include "./Stmt.hpp"

using namespace std;
using super::object;


Expression::Expression(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

object Expression::accept(Visitor& visitor) {

	return visitor.visitExpressionStmt(*this);
}


Print::Print(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

object Print::accept(Visitor& visitor) {

	return visitor.visitPrintStmt(*this);
}


