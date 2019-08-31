#include "my_listener.h"


void MyListener::enterMain(FormulaParser::MainContext *ctx) {
    root_ = std::make_shared<Root>();
}

void MyListener::exitMain(FormulaParser::MainContext *mainContext) {
    root_->SetRoot(built_args_.top());
    built_args_.pop();
    assert(built_args_.empty());
}

void MyListener::enterLiteral(FormulaParser::LiteralContext *literalContext) {
    built_args_.push(std::make_shared<Number>(literalContext->NUMBER()->getText()));
}

void MyListener::enterUnaryOp(FormulaParser::UnaryOpContext *unaryOpContext) {
    unary_ops_.push(std::make_shared<UnaryOperation>(unaryOpContext->children[0]->getText()));
}

void MyListener::exitUnaryOp(FormulaParser::UnaryOpContext *unaryOpContext) {
    const auto arg = built_args_.top();
    built_args_.pop();
    const auto unary_op = unary_ops_.top();
    unary_ops_.pop();
    unary_op->SetArgument(arg);
    built_args_.push(unary_op);
}

void MyListener::enterBinaryOp(FormulaParser::BinaryOpContext *binaryOpContext) {
    binary_ops_.push(std::make_shared<BinaryOperation>(binaryOpContext->children[1]->getText()));
}

void MyListener::exitBinaryOp(FormulaParser::BinaryOpContext *binaryOpContext) {
    const auto arg1 = built_args_.top();
    built_args_.pop();
    const auto arg2 = built_args_.top();
    built_args_.pop();

    const auto binary_op = binary_ops_.top();
    binary_ops_.pop();
    binary_op->SetLeft(arg2);
    binary_op->SetRight(arg1);
    built_args_.push(binary_op);
}

void MyListener::enterCell(FormulaParser::CellContext *cellContext) {
    built_args_.push(std::make_shared<Cell>(cellContext->CELL()->getText()));
}