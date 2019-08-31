
// Generated from Formula.g4 by ANTLR 4.7.2


#include "FormulaListener.h"

#include "FormulaParser.h"


using namespace antlrcpp;
using namespace antlr4;

FormulaParser::FormulaParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

FormulaParser::~FormulaParser() {
  delete _interpreter;
}

std::string FormulaParser::getGrammarFileName() const {
  return "Formula.g4";
}

const std::vector<std::string>& FormulaParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& FormulaParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- MainContext ------------------------------------------------------------------

FormulaParser::MainContext::MainContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

FormulaParser::ExprContext* FormulaParser::MainContext::expr() {
  return getRuleContext<FormulaParser::ExprContext>(0);
}

tree::TerminalNode* FormulaParser::MainContext::EOF() {
  return getToken(FormulaParser::EOF, 0);
}


size_t FormulaParser::MainContext::getRuleIndex() const {
  return FormulaParser::RuleMain;
}

void FormulaParser::MainContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMain(this);
}

void FormulaParser::MainContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMain(this);
}

FormulaParser::MainContext* FormulaParser::main() {
  MainContext *_localctx = _tracker.createInstance<MainContext>(_ctx, getState());
  enterRule(_localctx, 0, FormulaParser::RuleMain);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(4);
    expr(0);
    setState(5);
    match(FormulaParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

FormulaParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t FormulaParser::ExprContext::getRuleIndex() const {
  return FormulaParser::RuleExpr;
}

void FormulaParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- UnaryOpContext ------------------------------------------------------------------

FormulaParser::ExprContext* FormulaParser::UnaryOpContext::expr() {
  return getRuleContext<FormulaParser::ExprContext>(0);
}

tree::TerminalNode* FormulaParser::UnaryOpContext::ADD() {
  return getToken(FormulaParser::ADD, 0);
}

tree::TerminalNode* FormulaParser::UnaryOpContext::SUB() {
  return getToken(FormulaParser::SUB, 0);
}

FormulaParser::UnaryOpContext::UnaryOpContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::UnaryOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryOp(this);
}
void FormulaParser::UnaryOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryOp(this);
}
//----------------- ParensContext ------------------------------------------------------------------

FormulaParser::ExprContext* FormulaParser::ParensContext::expr() {
  return getRuleContext<FormulaParser::ExprContext>(0);
}

FormulaParser::ParensContext::ParensContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::ParensContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParens(this);
}
void FormulaParser::ParensContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParens(this);
}
//----------------- LiteralContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::LiteralContext::NUMBER() {
  return getToken(FormulaParser::NUMBER, 0);
}

FormulaParser::LiteralContext::LiteralContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::LiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral(this);
}
void FormulaParser::LiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral(this);
}
//----------------- CellContext ------------------------------------------------------------------

tree::TerminalNode* FormulaParser::CellContext::CELL() {
  return getToken(FormulaParser::CELL, 0);
}

FormulaParser::CellContext::CellContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::CellContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCell(this);
}
void FormulaParser::CellContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCell(this);
}
//----------------- BinaryOpContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::BinaryOpContext::expr() {
  return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext* FormulaParser::BinaryOpContext::expr(size_t i) {
  return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode* FormulaParser::BinaryOpContext::MUL() {
  return getToken(FormulaParser::MUL, 0);
}

tree::TerminalNode* FormulaParser::BinaryOpContext::DIV() {
  return getToken(FormulaParser::DIV, 0);
}

tree::TerminalNode* FormulaParser::BinaryOpContext::ADD() {
  return getToken(FormulaParser::ADD, 0);
}

tree::TerminalNode* FormulaParser::BinaryOpContext::SUB() {
  return getToken(FormulaParser::SUB, 0);
}

FormulaParser::BinaryOpContext::BinaryOpContext(ExprContext *ctx) { copyFrom(ctx); }

void FormulaParser::BinaryOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBinaryOp(this);
}
void FormulaParser::BinaryOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FormulaListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBinaryOp(this);
}

FormulaParser::ExprContext* FormulaParser::expr() {
   return expr(0);
}

FormulaParser::ExprContext* FormulaParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  FormulaParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  FormulaParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 2;
  enterRecursionRule(_localctx, 2, FormulaParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(16);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case FormulaParser::T__0: {
        _localctx = _tracker.createInstance<ParensContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(8);
        match(FormulaParser::T__0);
        setState(9);
        expr(0);
        setState(10);
        match(FormulaParser::T__1);
        break;
      }

      case FormulaParser::ADD:
      case FormulaParser::SUB: {
        _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(12);
        _la = _input->LA(1);
        if (!(_la == FormulaParser::ADD

        || _la == FormulaParser::SUB)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(13);
        expr(5);
        break;
      }

      case FormulaParser::CELL: {
        _localctx = _tracker.createInstance<CellContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(14);
        match(FormulaParser::CELL);
        break;
      }

      case FormulaParser::NUMBER: {
        _localctx = _tracker.createInstance<LiteralContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(15);
        match(FormulaParser::NUMBER);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(26);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(24);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(18);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(19);
          _la = _input->LA(1);
          if (!(_la == FormulaParser::MUL

          || _la == FormulaParser::DIV)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(20);
          expr(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(21);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(22);
          _la = _input->LA(1);
          if (!(_la == FormulaParser::ADD

          || _la == FormulaParser::SUB)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(23);
          expr(4);
          break;
        }

        } 
      }
      setState(28);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool FormulaParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 1: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool FormulaParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 4);
    case 1: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> FormulaParser::_decisionToDFA;
atn::PredictionContextCache FormulaParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN FormulaParser::_atn;
std::vector<uint16_t> FormulaParser::_serializedATN;

std::vector<std::string> FormulaParser::_ruleNames = {
  "main", "expr"
};

std::vector<std::string> FormulaParser::_literalNames = {
  "", "'('", "')'", "", "'+'", "'-'", "'*'", "'/'"
};

std::vector<std::string> FormulaParser::_symbolicNames = {
  "", "", "", "NUMBER", "ADD", "SUB", "MUL", "DIV", "CELL", "WS"
};

dfa::Vocabulary FormulaParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> FormulaParser::_tokenNames;

FormulaParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0xb, 0x20, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x13, 0xa, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x1b, 0xa, 
    0x3, 0xc, 0x3, 0xe, 0x3, 0x1e, 0xb, 0x3, 0x3, 0x3, 0x2, 0x3, 0x4, 0x4, 
    0x2, 0x4, 0x2, 0x4, 0x3, 0x2, 0x6, 0x7, 0x3, 0x2, 0x8, 0x9, 0x2, 0x22, 
    0x2, 0x6, 0x3, 0x2, 0x2, 0x2, 0x4, 0x12, 0x3, 0x2, 0x2, 0x2, 0x6, 0x7, 
    0x5, 0x4, 0x3, 0x2, 0x7, 0x8, 0x7, 0x2, 0x2, 0x3, 0x8, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x9, 0xa, 0x8, 0x3, 0x1, 0x2, 0xa, 0xb, 0x7, 0x3, 0x2, 0x2, 
    0xb, 0xc, 0x5, 0x4, 0x3, 0x2, 0xc, 0xd, 0x7, 0x4, 0x2, 0x2, 0xd, 0x13, 
    0x3, 0x2, 0x2, 0x2, 0xe, 0xf, 0x9, 0x2, 0x2, 0x2, 0xf, 0x13, 0x5, 0x4, 
    0x3, 0x7, 0x10, 0x13, 0x7, 0xa, 0x2, 0x2, 0x11, 0x13, 0x7, 0x5, 0x2, 
    0x2, 0x12, 0x9, 0x3, 0x2, 0x2, 0x2, 0x12, 0xe, 0x3, 0x2, 0x2, 0x2, 0x12, 
    0x10, 0x3, 0x2, 0x2, 0x2, 0x12, 0x11, 0x3, 0x2, 0x2, 0x2, 0x13, 0x1c, 
    0x3, 0x2, 0x2, 0x2, 0x14, 0x15, 0xc, 0x6, 0x2, 0x2, 0x15, 0x16, 0x9, 
    0x3, 0x2, 0x2, 0x16, 0x1b, 0x5, 0x4, 0x3, 0x7, 0x17, 0x18, 0xc, 0x5, 
    0x2, 0x2, 0x18, 0x19, 0x9, 0x2, 0x2, 0x2, 0x19, 0x1b, 0x5, 0x4, 0x3, 
    0x6, 0x1a, 0x14, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x17, 0x3, 0x2, 0x2, 0x2, 
    0x1b, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x1c, 
    0x1d, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x5, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x1c, 
    0x3, 0x2, 0x2, 0x2, 0x5, 0x12, 0x1a, 0x1c, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

FormulaParser::Initializer FormulaParser::_init;
