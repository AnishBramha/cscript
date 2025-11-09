#include "./AstPrinter.hpp"
#include "./Expr.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <sysexits.h>
#include <type_traits>
#include <vector>
#include "../superclass/super.hpp"

#define ERR_MESSAGE "STRING NOT RETURNED" 


std::string AstPrinter::print(Expr& expr) {

    object literal = expr.accept(*this);

    if (auto* str_ptr = std::get_if<std::string>(&literal))
        return *str_ptr;

    std::cerr << "AstPrinter::print: " << ERR_MESSAGE << std::endl;

    std::exit(EX_DATAERR);
}


std::string AstPrinter::parenthesise(const std::string& name, const std::vector<Expr*>& exprs) {

    std::stringstream builder;

    builder << "(" << name;

    for (auto* expr : exprs) {

        object literal = expr->accept(*this);

        std::string* str_ptr = nullptr;

        str_ptr = std::get_if<std::string>(&literal);

        if (!str_ptr) {

            std::cerr << "AstPrinter::parenthesise: " << ERR_MESSAGE << std::endl;

            std::exit(EX_DATAERR);
        }

        builder << " " << *str_ptr;
    }

    builder << ")";

    return builder.str();
}


object AstPrinter::visitBinaryExpr(const Binary& expr) {

    return this->parenthesise(expr.oprtor.lexeme, {expr.left.get(), expr.right.get()});
}


object AstPrinter::visitGroupingExpr(const Grouping& expr) {

    return this->parenthesise("group", {expr.expr.get()});
}


object AstPrinter::visitLiteralExpr(const Literal& expr) {

    return std::visit(

        [](const auto& value) -> std::string {

            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, std::nullptr_t>)
                return "nil";

            else if constexpr (std::is_same_v<T, std::string>)
                return value;

            else if constexpr (std::is_same_v<T, double>)
                return std::to_string(value);

            else if constexpr (std::is_same_v<T, bool>)
                return value ? "true" : "false";

            else {

                std::cerr << "AstPrinter::visitLiteralExpr: " << ERR_MESSAGE << std::endl;

                std::exit(EX_DATAERR);
            }

        }, expr.value
    );
}


object AstPrinter::visitUnaryExpr(const Unary& expr) {

    return this->parenthesise(expr.oprtor.lexeme, {expr.right.get()});
}



















