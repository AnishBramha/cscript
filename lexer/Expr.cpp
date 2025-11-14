#include "../superclass/super.hpp"

#include "./Expr.hpp"

using namespace std;
using super::object;


Assign::Assign(const Token& name, std::unique_ptr<Expr> value) : name(name), value(std::move(value)) {}

object Assign::accept(Visitor& visitor) {

	return visitor.visitAssignExpr(*this);
}


Binary::Binary(std::unique_ptr<Expr> left, const Token& oprtor, std::unique_ptr<Expr> right) : left(std::move(left)), oprtor(oprtor), right(std::move(right)) {}

object Binary::accept(Visitor& visitor) {

	return visitor.visitBinaryExpr(*this);
}


Call::Call(std::unique_ptr<Expr> callee, const Token& paren, vector<unique_ptr<Expr>>&& args) : callee(std::move(callee)), paren(paren), args(std::move(args)) {}

object Call::accept(Visitor& visitor) {

	return visitor.visitCallExpr(*this);
}


Grouping::Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

object Grouping::accept(Visitor& visitor) {

	return visitor.visitGroupingExpr(*this);
}


Literal::Literal(const object& value) : value(value) {}

object Literal::accept(Visitor& visitor) {

	return visitor.visitLiteralExpr(*this);
}


Logical::Logical(std::unique_ptr<Expr> left, const Token& operatr, std::unique_ptr<Expr> right) : left(std::move(left)), operatr(operatr), right(std::move(right)) {}

object Logical::accept(Visitor& visitor) {

	return visitor.visitLogicalExpr(*this);
}


Unary::Unary(const Token& oprtor, std::unique_ptr<Expr> right) : oprtor(oprtor), right(std::move(right)) {}

object Unary::accept(Visitor& visitor) {

	return visitor.visitUnaryExpr(*this);
}


Variable::Variable(const Token& name) : name(name) {}

object Variable::accept(Visitor& visitor) {

	return visitor.visitVariableExpr(*this);
}


