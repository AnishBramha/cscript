#include "./Visitor.hpp"
#include "./Expr.hpp"
#include "./Stmt.hpp"

using super::object;

object Visitor::visitBinaryExpr(const Binary& expr) {

	return object();
}

object Visitor::visitGroupingExpr(const Grouping& expr) {

	return object();
}

object Visitor::visitLiteralExpr(const Literal& expr) {

	return object();
}

object Visitor::visitUnaryExpr(const Unary& expr) {

	return object();
}

object Visitor::visitExpressionStmt(const Expression& stmt) {

	return object();
}

object Visitor::visitPrintStmt(const Print& stmt) {

	return object();
}

