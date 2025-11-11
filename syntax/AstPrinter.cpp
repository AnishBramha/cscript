#include "./AstPrinter.hpp"
#include "./Expr.hpp"
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <string>
#include <sysexits.h>
#include <vector>
#include "../superclass/super.hpp"

#define ERR_MESSAGE "STRING NOT RETURNED" 


std::string AstPrinter::print(Expr& expr) {

    return expr.accept(*this).as_string();
}


std::string AstPrinter::parenthesise(const std::string& name, const std::vector<Expr*>& exprs) {

    std::stringstream builder;

    builder << "(" << name;

    for (auto* expr : exprs)
        builder << " " << expr->accept(*this).as_string();

    builder << ")";

    return builder.str();
}


super::object AstPrinter::visitBinaryExpr(const Binary& expr) {

    return this->parenthesise(expr.oprtor.lexeme, {expr.left.get(), expr.right.get()});
}


super::object AstPrinter::visitGroupingExpr(const Grouping& expr) {

    return this->parenthesise("group", {expr.expr.get()});
}


super::object AstPrinter::visitLiteralExpr(const Literal& expr) {

    return expr.value.to_string();
}


super::object AstPrinter::visitUnaryExpr(const Unary& expr) {

    return this->parenthesise(expr.oprtor.lexeme, {expr.right.get()});
}



















