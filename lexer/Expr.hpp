#pragma once

#include <memory>
#include "../tokeniser/Token.hpp"
#include "../superclass/super.hpp"

using super::object;

class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor;

class Expr {

	public:

		virtual object accept(Visitor& visitor) = 0;

		virtual ~Expr() = default;
};


class Visitor {

	public:

		virtual object visitBinaryExpr(const Binary& expr) = 0;
		virtual object visitGroupingExpr(const Grouping& expr) = 0;
		virtual object visitLiteralExpr(const Literal& expr) = 0;
		virtual object visitUnaryExpr(const Unary& expr) = 0;

		virtual ~Visitor() = default;
};


class Binary : public Expr {

	public:

		const std::unique_ptr<Expr> left;
		const Token oprtor;
		const std::unique_ptr<Expr> right;

		Binary(std::unique_ptr<Expr> left, const Token& oprtor, std::unique_ptr<Expr> right);

		object accept(Visitor& visitor) override;

};


class Grouping : public Expr {

	public:

		const std::unique_ptr<Expr> expr;

		Grouping(std::unique_ptr<Expr> expr);

		object accept(Visitor& visitor) override;

};


class Literal : public Expr {

	public:

		const object value;

		Literal(const object& value);

		object accept(Visitor& visitor) override;

};


class Unary : public Expr {

	public:

		const Token oprtor;
		const std::unique_ptr<Expr> right;

		Unary(const Token& oprtor, std::unique_ptr<Expr> right);

		object accept(Visitor& visitor) override;

};







