#pragma once
#include "FormulaBaseListener.h"
#include "tree.h"
#include <stack>


class MyListener : public FormulaBaseListener {
public:
    void enterMain(FormulaParser::MainContext *mainContext) override;

    void exitMain(FormulaParser::MainContext *mainContext) override;

    void enterLiteral(FormulaParser::LiteralContext *literalContext) override;

    void enterUnaryOp(FormulaParser::UnaryOpContext *unaryOpContext) override;

    void exitUnaryOp(FormulaParser::UnaryOpContext *unaryOpContext) override;

    void enterBinaryOp(FormulaParser::BinaryOpContext *binaryOpContext) override;

    void exitBinaryOp(FormulaParser::BinaryOpContext *binaryOpContext) override;

    void enterCell(FormulaParser::CellContext *cellContext) override;

    [[nodiscard]] double getResult() const {
        return root_->Apply();
    }

    [[nodiscard]] std::string getExpression() const {
        return root_->toString();
    }

private:
    std::shared_ptr<Root> root_;
    std::stack<std::shared_ptr<UnaryOperation>> unary_ops_;
    std::stack<std::shared_ptr<BinaryOperation>> binary_ops_;
    std::stack<std::shared_ptr<INode>> built_args_;
};