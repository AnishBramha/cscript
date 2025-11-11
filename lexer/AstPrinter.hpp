#pragma once

#include "./Expr.hpp"
#include <string>
#include <vector>
#include "../superclass/super.hpp"


class AstPrinter : public Visitor {

    private:

        std::string parenthesise(const std::string& name, const std::vector<Expr*>& exprs);

    public:

        std::string print(Expr& expr);

        super::object visitBinaryExpr(const Binary&) override;
        super::object visitGroupingExpr(const Grouping&) override;
        super::object visitLiteralExpr(const Literal&) override;
        super::object visitUnaryExpr(const Unary&) override;
};









