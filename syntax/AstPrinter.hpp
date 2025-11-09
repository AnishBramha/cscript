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

        object visitBinaryExpr(const Binary&) override;
        object visitGroupingExpr(const Grouping&) override;
        object visitLiteralExpr(const Literal&) override;
        object visitUnaryExpr(const Unary&) override;
};









