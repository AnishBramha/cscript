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
class Get;
class Grouping;
class Literal;
class Logical;
class Set;
class Super;
class This;
class Unary;
class Variable;
class Block;
class Class;
class Expression;
class Function;
class If;
class Print;
class Println;
class Return;
class Var;
class While;


class Visitor {

	public:

		virtual object visitAssignExpr(const Assign&);
		virtual object visitBinaryExpr(const Binary&);
		virtual object visitCallExpr(const Call&);
		virtual object visitGetExpr(const Get&);
		virtual object visitGroupingExpr(const Grouping&);
		virtual object visitLiteralExpr(const Literal&);
		virtual object visitLogicalExpr(const Logical&);
		virtual object visitSetExpr(const Set&);
		virtual object visitSuperExpr(const Super&);
		virtual object visitThisExpr(const This&);
		virtual object visitUnaryExpr(const Unary&);
		virtual object visitVariableExpr(const Variable&);

		virtual object visitBlockStmt(const Block&);
		virtual object visitClassStmt(const Class&);
		virtual object visitExpressionStmt(const Expression&);
		virtual object visitFunctionStmt(const Function&);
		virtual object visitIfStmt(const If&);
		virtual object visitPrintStmt(const Print&);
		virtual object visitPrintlnStmt(const Println&);
		virtual object visitReturnStmt(const Return&);
		virtual object visitVarStmt(const Var&);
		virtual object visitWhileStmt(const While&);


		virtual ~Visitor() = default;
};




