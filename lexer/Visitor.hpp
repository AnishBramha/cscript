#pragma once

#include "../superclass/super.hpp"

#include <vector>

#include <memory>

using super::object;

using std::vector;

using std::unique_ptr;

class Assign;
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
class Block;
class Expression;
class Print;
class Var;


class Visitor {

	public:

		virtual object visitAssignExpr(const Assign& expr);
		virtual object visitBinaryExpr(const Binary& expr);
		virtual object visitGroupingExpr(const Grouping& expr);
		virtual object visitLiteralExpr(const Literal& expr);
		virtual object visitUnaryExpr(const Unary& expr);
		virtual object visitVariableExpr(const Variable& expr);

		virtual object visitBlockStmt(const Block& stmt);
		virtual object visitExpressionStmt(const Expression& stmt);
		virtual object visitPrintStmt(const Print& stmt);
		virtual object visitVarStmt(const Var& stmt);


		virtual ~Visitor() = default;
};




