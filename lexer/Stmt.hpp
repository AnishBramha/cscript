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


class Block : public Stmt {

	public:

		const vector<unique_ptr<Stmt>> statements;

		Block(vector<unique_ptr<Stmt>>&& statements);

		object accept(Visitor& visitor) override;

};


class Expression : public Stmt {

	public:

		const std::unique_ptr<Expr> expr;

		Expression(std::unique_ptr<Expr> expr);

		object accept(Visitor& visitor) override;

};


class Function : public Stmt {

	public:

		const Token name;
		const vector<Token> params;
		const vector<unique_ptr<Stmt>> body;

		Function(const Token& name, vector<Token>&& params, vector<unique_ptr<Stmt>>&& body);

		object accept(Visitor& visitor) override;
};


class If : public Stmt {

	public:

		const std::unique_ptr<Expr> condition;
		const std::unique_ptr<Stmt> thenBranch;
		const std::unique_ptr<Stmt> elseBranch;

		If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);

		object accept(Visitor& visitor) override;

};


class Print : public Stmt {

	public:

		const std::unique_ptr<Expr> expr;

		Print(std::unique_ptr<Expr> expr);

		object accept(Visitor& visitor) override;

};


class Println : public Stmt {

	public:

		const std::unique_ptr<Expr> expr;

		Println(std::unique_ptr<Expr> expr);

		object accept(Visitor& visitor) override;

};


class Var : public Stmt {

	public:

		const Token name;
		const std::unique_ptr<Expr> initialiser;

		Var(const Token& name, std::unique_ptr<Expr> initialiser);

		object accept(Visitor& visitor) override;

};


class While : public Stmt {

	public:

		const std::unique_ptr<Expr> condition;
		const std::unique_ptr<Stmt> body;

		While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);

		object accept(Visitor& visitor) override;

};







