#include "../superclass/super.hpp"

#include "./Expr.hpp"

using namespace std;

Binary::Binary(std::unique_ptr<Expr> left, const Token& oprtor, std::unique_ptr<Expr> right) : left(std::move(left)), oprtor(oprtor), right(std::move(right)) {}

super::object Binary::accept(Visitor& visitor) {

	return visitor.visitBinaryExpr(*this);
}


Grouping::Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

super::object Grouping::accept(Visitor& visitor) {

	return visitor.visitGroupingExpr(*this);
}


Literal::Literal(const super::object& value) : value(value) {}

super::object Literal::accept(Visitor& visitor) {

	return visitor.visitLiteralExpr(*this);
}


Unary::Unary(const Token& oprtor, std::unique_ptr<Expr> right) : oprtor(oprtor), right(std::move(right)) {}

super::object Unary::accept(Visitor& visitor) {

	return visitor.visitUnaryExpr(*this);
}


