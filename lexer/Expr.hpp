#pragma once

#include <memory>
#include "../tokeniser/Token.hpp"
#include "../superclass/super.hpp"
#include "./Visitor.hpp"

using super::object;


class Expr {

	public:

		virtual object accept(Visitor& visitor) = 0;

		virtual ~Expr() = default;
};


class Assign : public Expr {

	public:

		const Token name;
		const std::unique_ptr<Expr> value;

		Assign(const Token& name, std::unique_ptr<Expr> value);

		object accept(Visitor& visitor) override;

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


class Variable : public Expr {

	public:

		const Token name;

		Variable(const Token& name);

		object accept(Visitor& visitor) override;

};







