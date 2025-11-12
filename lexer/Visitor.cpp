#include "./Visitor.hpp"
#include "./Expr.hpp"
#include "./Stmt.hpp"

#include <vector>

#include <memory>

using super::object;

using std::vector;

using std::unique_ptr;

object Visitor::visitAssignExpr(const Assign& expr) {

	return object();
}

object Visitor::visitBinaryExpr(const Binary& expr) {

	return object();
}

object Visitor::visitGroupingExpr(const Grouping& expr) {

	return object();
}

object Visitor::visitLiteralExpr(const Literal& expr) {

	return object();
}

object Visitor::visitLogicalExpr(const Logical& expr) {

	return object();
}

object Visitor::visitUnaryExpr(const Unary& expr) {

	return object();
}

object Visitor::visitVariableExpr(const Variable& expr) {

	return object();
}

object Visitor::visitBlockStmt(const Block& stmt) {

	return object();
}

object Visitor::visitExpressionStmt(const Expression& stmt) {

	return object();
}

object Visitor::visitIfStmt(const If& stmt) {

	return object();
}

object Visitor::visitPrintStmt(const Print& stmt) {

	return object();
}

object Visitor::visitPrintlnStmt(const Println& stmt) {

	return object();
}

object Visitor::visitVarStmt(const Var& stmt) {

	return object();
}

object Visitor::visitWhileStmt(const While& stmt) {

	return object();
}

