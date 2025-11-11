#pragma once

#include <memory>
#include "../tokeniser/Token.hpp"
#include "./Expr.hpp"
#include "../superclass/super.hpp"
#include "./Visitor.hpp"

using super::object;


class Stmt {

	public:

		virtual object accept(Visitor& visitor) = 0;

		virtual ~Stmt() = default;
};


class Expression : public Stmt {

	public:

		const std::unique_ptr<Expr> expr;

		Expression(std::unique_ptr<Expr> expr);

		object accept(Visitor& visitor) override;

};


class Print : public Stmt {

	public:

		const std::unique_ptr<Expr> expr;

		Print(std::unique_ptr<Expr> expr);

		object accept(Visitor& visitor) override;

};







