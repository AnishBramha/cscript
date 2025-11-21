#include "./Visitor.hpp"
#include "./Expr.hpp"
#include "./Stmt.hpp"

#include <vector>

#include <memory>

using super::object;

using std::vector;

using std::unique_ptr;

object Visitor::visitAssignExpr(const Assign&) {

	return object();
}

object Visitor::visitBinaryExpr(const Binary&) {

	return object();
}

object Visitor::visitCallExpr(const Call&) {

	return object();
}

object Visitor::visitGroupingExpr(const Grouping&) {

	return object();
}

object Visitor::visitLiteralExpr(const Literal&) {

	return object();
}

object Visitor::visitLogicalExpr(const Logical&) {

	return object();
}

object Visitor::visitUnaryExpr(const Unary&) {

	return object();
}

object Visitor::visitVariableExpr(const Variable&) {

	return object();
}

object Visitor::visitBlockStmt(const Block&) {

	return object();
}

object Visitor::visitExpressionStmt(const Expression&) {

	return object();
}

object Visitor::visitFunctionStmt(const Function&) {

	return object();
}

object Visitor::visitIfStmt(const If&) {

	return object();
}

object Visitor::visitPrintStmt(const Print&) {

	return object();
}

object Visitor::visitPrintlnStmt(const Println&) {

	return object();
}

object Visitor::visitReturnStmt(const Return&) {

	return object();
}

object Visitor::visitVarStmt(const Var&) {

	return object();
}

object Visitor::visitWhileStmt(const While&) {

	return object();
}

