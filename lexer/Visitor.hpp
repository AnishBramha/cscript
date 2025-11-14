#pragma once

#include "../superclass/super.hpp"

#include <vector>

#include <memory>

using super::object;

using std::vector;

using std::unique_ptr;

class Assign;
class Binary;
class Call;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;
class Block;
class Expression;
class Function;
class If;
class Print;
class Println;
class Var;
class While;


class Visitor {

	public:

		virtual object visitAssignExpr(const Assign& expr);
		virtual object visitBinaryExpr(const Binary& expr);
		virtual object visitCallExpr(const Call& expr);
		virtual object visitGroupingExpr(const Grouping& expr);
		virtual object visitLiteralExpr(const Literal& expr);
		virtual object visitLogicalExpr(const Logical& expr);
		virtual object visitUnaryExpr(const Unary& expr);
		virtual object visitVariableExpr(const Variable& expr);

		virtual object visitBlockStmt(const Block& stmt);
		virtual object visitExpressionStmt(const Expression& stmt);
		virtual object visitFunctionStmt(const Function& stmt);
		virtual object visitIfStmt(const If& stmt);
		virtual object visitPrintStmt(const Print& stmt);
		virtual object visitPrintlnStmt(const Println& stmt);
		virtual object visitVarStmt(const Var& stmt);
		virtual object visitWhileStmt(const While& stmt);


		virtual ~Visitor() = default;
};




