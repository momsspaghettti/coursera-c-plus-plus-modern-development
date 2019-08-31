
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "FormulaListener.h"


/**
 * This class provides an empty implementation of FormulaListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  FormulaBaseListener : public FormulaListener {
public:

  virtual void enterMain(FormulaParser::MainContext * /*ctx*/) override { }
  virtual void exitMain(FormulaParser::MainContext * /*ctx*/) override { }

  virtual void enterUnaryOp(FormulaParser::UnaryOpContext * /*ctx*/) override { }
  virtual void exitUnaryOp(FormulaParser::UnaryOpContext * /*ctx*/) override { }

  virtual void enterParens(FormulaParser::ParensContext * /*ctx*/) override { }
  virtual void exitParens(FormulaParser::ParensContext * /*ctx*/) override { }

  virtual void enterLiteral(FormulaParser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(FormulaParser::LiteralContext * /*ctx*/) override { }

  virtual void enterCell(FormulaParser::CellContext * /*ctx*/) override { }
  virtual void exitCell(FormulaParser::CellContext * /*ctx*/) override { }

  virtual void enterBinaryOp(FormulaParser::BinaryOpContext * /*ctx*/) override { }
  virtual void exitBinaryOp(FormulaParser::BinaryOpContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

