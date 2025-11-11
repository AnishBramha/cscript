#pragma once

#include "../superclass/super.hpp"

using super::object;

class Binary;
class Grouping;
class Literal;
class Unary;
class Expression;
class Print;


class Visitor {

	public:

		virtual object visitBinaryExpr(const Binary& expr);
		virtual object visitGroupingExpr(const Grouping& expr);
		virtual object visitLiteralExpr(const Literal& expr);
		virtual object visitUnaryExpr(const Unary& expr);

		virtual object visitExpressionStmt(const Expression& stmt);
		virtual object visitPrintStmt(const Print& stmt);


		virtual ~Visitor() = default;
};




