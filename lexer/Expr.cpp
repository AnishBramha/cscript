#include "../superclass/super.hpp"

#include "./Expr.hpp"

using namespace std;
using super::object;


Binary::Binary(std::unique_ptr<Expr> left, const Token& oprtor, std::unique_ptr<Expr> right) : left(std::move(left)), oprtor(oprtor), right(std::move(right)) {}

object Binary::accept(Visitor& visitor) {

	return visitor.visitBinaryExpr(*this);
}


Grouping::Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

object Grouping::accept(Visitor& visitor) {

	return visitor.visitGroupingExpr(*this);
}


Literal::Literal(const object& value) : value(value) {}

object Literal::accept(Visitor& visitor) {

	return visitor.visitLiteralExpr(*this);
}


Unary::Unary(const Token& oprtor, std::unique_ptr<Expr> right) : oprtor(oprtor), right(std::move(right)) {}

object Unary::accept(Visitor& visitor) {

	return visitor.visitUnaryExpr(*this);
}


Variable::Variable(const Token& name) : name(name) {}

object Variable::accept(Visitor& visitor) {

	return visitor.visitVariableExpr(*this);
}


