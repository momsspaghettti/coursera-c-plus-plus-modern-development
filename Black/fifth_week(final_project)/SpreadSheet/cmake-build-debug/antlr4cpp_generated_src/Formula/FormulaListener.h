
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "FormulaParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by FormulaParser.
 */
class  FormulaListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterMain(FormulaParser::MainContext *ctx) = 0;
  virtual void exitMain(FormulaParser::MainContext *ctx) = 0;

  virtual void enterUnaryOp(FormulaParser::UnaryOpContext *ctx) = 0;
  virtual void exitUnaryOp(FormulaParser::UnaryOpContext *ctx) = 0;

  virtual void enterParens(FormulaParser::ParensContext *ctx) = 0;
  virtual void exitParens(FormulaParser::ParensContext *ctx) = 0;

  virtual void enterLiteral(FormulaParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(FormulaParser::LiteralContext *ctx) = 0;

  virtual void enterCell(FormulaParser::CellContext *ctx) = 0;
  virtual void exitCell(FormulaParser::CellContext *ctx) = 0;

  virtual void enterBinaryOp(FormulaParser::BinaryOpContext *ctx) = 0;
  virtual void exitBinaryOp(FormulaParser::BinaryOpContext *ctx) = 0;


};

