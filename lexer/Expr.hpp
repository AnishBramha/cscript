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


class Call : public Expr {

	public:

		const std::unique_ptr<Expr> callee;
		const Token paren;
		const vector<unique_ptr<Expr>> args;

		Call(std::unique_ptr<Expr> callee, const Token& paren, vector<unique_ptr<Expr>>&& args);

		object accept(Visitor& visitor) override;

};


class Get : public Expr {

	public:

		const std::unique_ptr<Expr> obj;
		const Token name;

		Get(std::unique_ptr<Expr> obj, const Token& name);

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


class Logical : public Expr {

	public:

		const std::unique_ptr<Expr> left;
		const Token operatr;
		const std::unique_ptr<Expr> right;

		Logical(std::unique_ptr<Expr> left, const Token& operatr, std::unique_ptr<Expr> right);

		object accept(Visitor& visitor) override;

};


class Set : public Expr {

	public:

		const std::unique_ptr<Expr> obj;
		const Token name;
		const std::unique_ptr<Expr> val;

		Set(std::unique_ptr<Expr> obj, const Token& name, std::unique_ptr<Expr> val);

		object accept(Visitor& visitor) override;

};


class This : public Expr {

	public:

		const Token keyword;

		This(const Token& keyword);

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







