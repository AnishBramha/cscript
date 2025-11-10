#pragma once

#include <memory>
#include "../tokeniser/Token.hpp"
#include "../superclass/super.hpp"

class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor;

class Expr {

	public:

		virtual super::object accept(Visitor& visitor) = 0;

		virtual ~Expr() = default;
};


class Visitor {

	public:

		virtual super::object visitBinaryExpr(const Binary& expr) = 0;
		virtual super::object visitGroupingExpr(const Grouping& expr) = 0;
		virtual super::object visitLiteralExpr(const Literal& expr) = 0;
		virtual super::object visitUnaryExpr(const Unary& expr) = 0;

		virtual ~Visitor() = default;
};


class Binary : public Expr {

	public:

		const std::unique_ptr<Expr> left;
		const Token oprtor;
		const std::unique_ptr<Expr> right;

		Binary(std::unique_ptr<Expr> left, const Token& oprtor, std::unique_ptr<Expr> right);

		super::object accept(Visitor& visitor) override;

};


class Grouping : public Expr {

	public:

		const std::unique_ptr<Expr> expr;

		Grouping(std::unique_ptr<Expr> expr);

		super::object accept(Visitor& visitor) override;

};


class Literal : public Expr {

	public:

		const super::object value;

		Literal(const super::object& value);

		super::object accept(Visitor& visitor) override;

};


class Unary : public Expr {

	public:

		const Token oprtor;
		const std::unique_ptr<Expr> right;

		Unary(const Token& oprtor, std::unique_ptr<Expr> right);

		super::object accept(Visitor& visitor) override;

};







