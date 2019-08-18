#include "parse.h"
#include "statement.h"
#include "lexer.h"
#include "comparators.h"
#include <string>
#include <vector>


namespace TokenType = Parse::TokenType;

namespace {
    bool operator==(const Parse::Token &token, char c) {
        auto p = token.TryAs<TokenType::Char>();
        return p && p->value == c;
    }

    bool operator!=(const Parse::Token &token, char c) {
        return !(token == c);
    }

}

class Parser {
public:
    explicit Parser(Parse::Lexer &lexer) : lexer(lexer) {
    }

    // Program -> eps
    //          | Statement \n Program
    std::unique_ptr<Ast::Statement> ParseProgram() {
        auto result = std::make_unique<Ast::Compound>();
        while (!lexer.CurrentToken().Is<TokenType::Eof>()) {
            result->AddStatement(ParseStatement());
        }

        return result;
    }

private:
    Parse::Lexer &lexer;
    Runtime::Closure declared_classes;

    // Suite -> NEWLINE INDENT (Statement)+ DEDENT
    std::unique_ptr<Ast::Statement> ParseSuite() {
        lexer.Expect<TokenType::Newline>();
        lexer.ExpectNext<TokenType::Indent>();

        lexer.NextToken();

        auto result = std::make_unique<Ast::Compound>();
        while (!lexer.CurrentToken().Is<TokenType::Dedent>()) {
            result->AddStatement(ParseStatement());
        }

        lexer.Expect<TokenType::Dedent>();
        lexer.NextToken();

        return result;
    }

    // Methods -> [def id(Params) : Suite]*
    std::vector<Runtime::Method> ParseMethods() {
        std::vector<Runtime::Method> result;

        while (lexer.CurrentToken().Is<TokenType::Def>()) {
            Runtime::Method m;

            m.name = lexer.ExpectNext<TokenType::Id>().value;
            lexer.ExpectNext<TokenType::Char>('(');

            if (lexer.NextToken().Is<TokenType::Id>()) {
                m.formal_params.push_back(lexer.Expect<TokenType::Id>().value);
                while (lexer.NextToken() == ',') {
                    m.formal_params.push_back(lexer.ExpectNext<TokenType::Id>().value);
                }
            }

            lexer.Expect<TokenType::Char>(')');
            lexer.ExpectNext<TokenType::Char>(':');
            lexer.NextToken();

            m.body = ParseSuite();

            result.push_back(std::move(m));
        }
        return result;
    }

    // ClassDefinition -> Id ['(' Id ')'] : new_line indent MethodList dedent
    std::unique_ptr<Ast::Statement> ParseClassDefinition() {
        std::string class_name = lexer.Expect<TokenType::Id>().value;

        lexer.NextToken();

        const Runtime::Class *base_class = nullptr;
        if (lexer.CurrentToken() == '(') {
            auto name = lexer.ExpectNext<TokenType::Id>().value;
            lexer.ExpectNext<TokenType::Char>(')');
            lexer.NextToken();

            if (auto it = declared_classes.find(name); it == declared_classes.end()) {
                throw ParseError("Base class " + name + " not found for class " + class_name);
            } else {
                base_class = dynamic_cast<const Runtime::Class *>(it->second.Get());
            }
        }

        lexer.Expect<TokenType::Char>(':');
        lexer.ExpectNext<TokenType::Newline>();
        lexer.ExpectNext<TokenType::Indent>();
        lexer.ExpectNext<TokenType::Def>();
        std::vector<Runtime::Method> methods = ParseMethods();

        lexer.Expect<TokenType::Dedent>();
        lexer.NextToken();

        auto[it, inserted] = declared_classes.insert({
            class_name,
            ObjectHolder::Own(
                    Runtime::Class(class_name, std::move(methods),
                            base_class
                            ))
        });

        if (!inserted) {
            throw ParseError("Class " + class_name + " already exists");
        }

        return std::make_unique<Ast::ClassDefinition>(it->second);
    }

    std::vector<std::string> ParseDottedIds() {
        std::vector<std::string> result(1, lexer.Expect<TokenType::Id>().value);

        while (lexer.NextToken() == '.') {
            result.push_back(lexer.ExpectNext<TokenType::Id>().value);
        }

        return result;
    }

    //  AssgnOrCall -> DottedIds = Expr
    //               | DottedIds '(' ExprList ')'
    std::unique_ptr<Ast::Statement> ParseAssignmentOrCall() {
        lexer.Expect<TokenType::Id>();

        std::vector<std::string> id_list = ParseDottedIds();
        std::string last_name = id_list.back();
        id_list.pop_back();

        if (lexer.CurrentToken() == '=') {
            lexer.NextToken();

            if (id_list.empty()) {
                return std::make_unique<Ast::Assignment>(std::move(last_name), ParseTest());
            } else {
                return std::make_unique<Ast::FieldAssignment>(
                        Ast::VariableValue{std::move(id_list)}, std::move(last_name), ParseTest()
                );
            }
        } else {
            lexer.Expect<TokenType::Char>('(');
            lexer.NextToken();

            if (id_list.empty()) {
                throw ParseError("Mython doesn't support functions, only methods: " + last_name);
            }


            std::vector<std::unique_ptr<Ast::Statement>> args;
            if (lexer.CurrentToken() != ')') {
                args = ParseTestList();
            }
            lexer.Expect<TokenType::Char>(')');
            lexer.NextToken();

            return std::make_unique<Ast::MethodCall>(
                    std::make_unique<Ast::VariableValue>(std::move(id_list)),
                    std::move(last_name),
                    std::move(args)
            );
        }
    }

    // Expr -> Adder ['+'/'-' Adder]*
    std::unique_ptr<Ast::Statement> ParseExpression() {
        std::unique_ptr<Ast::Statement> result = ParseAdder();
        while (lexer.CurrentToken() == '+' || lexer.CurrentToken() == '-') {
            char op = lexer.CurrentToken().As<TokenType::Char>().value;
            lexer.NextToken();

            if (op == '+') {
                result = std::make_unique<Ast::Add>(std::move(result), ParseAdder());
            } else {
                result = std::make_unique<Ast::Sub>(std::move(result), ParseAdder());
            }
        }
        return result;
    }

    // Adder -> Mult ['*'/'/' Mult]*
    std::unique_ptr<Ast::Statement> ParseAdder() {
        std::unique_ptr<Ast::Statement> result = ParseMult();
        while (lexer.CurrentToken() == '*' || lexer.CurrentToken() == '/') {
            char op = lexer.CurrentToken().As<TokenType::Char>().value;
            lexer.NextToken();

            if (op == '*') {
                result = std::make_unique<Ast::Mult>(std::move(result), ParseMult());
            } else {
                result = std::make_unique<Ast::Div>(std::move(result), ParseMult());
            }
        }
        return result;
    }

    // Mult -> '(' Expr ')'
    //       | NUMBER
    //       | '-' Mult
    //       | STRING
    //       | NONE
    //       | TRUE
    //       | FALSE
    //       | DottedIds '(' ExprList ')'
    //       | DottedIds
    std::unique_ptr<Ast::Statement> ParseMult() {
        if (lexer.CurrentToken() == '(') {
            lexer.NextToken();
            auto result = ParseTest();
            lexer.Expect<TokenType::Char>(')');
            lexer.NextToken();
            return result;
        } else if (lexer.CurrentToken() == '-') {
            lexer.NextToken();
            return std::make_unique<Ast::Mult>(
                    ParseMult(),
                    std::make_unique<Ast::NumericConst>(-1)
            );
        } else if (auto num = lexer.CurrentToken().TryAs<TokenType::Number>()) {
            int result = num->value;
            lexer.NextToken();
            return std::make_unique<Ast::NumericConst>(result);
        } else if (auto str = lexer.CurrentToken().TryAs<TokenType::String>()) {
            std::string result = str->value;
            lexer.NextToken();
            return std::make_unique<Ast::StringConst>(std::move(result));
        } else if (lexer.CurrentToken().Is<TokenType::True>()) {
            lexer.NextToken();
            return std::make_unique<Ast::BoolConst>(Runtime::Bool(true));
        } else if (lexer.CurrentToken().Is<TokenType::False>()) {
            lexer.NextToken();
            return std::make_unique<Ast::BoolConst>(Runtime::Bool(false));
        } else if (lexer.CurrentToken().Is<TokenType::None>()) {
            lexer.NextToken();
            return std::make_unique<Ast::None>();
        } else {
            std::vector<std::string> names = ParseDottedIds();

            if (lexer.CurrentToken() == '(') {
                // various calls
                std::vector<std::unique_ptr<Ast::Statement>> args;
                if (lexer.NextToken() != ')') {
                    args = ParseTestList();
                }
                lexer.Expect<TokenType::Char>(')');
                lexer.NextToken();

                auto method_name = names.back();
                names.pop_back();

                if (!names.empty()) {
                    return std::make_unique<Ast::MethodCall>(
                            std::make_unique<Ast::VariableValue>(std::move(names)),
                            std::move(method_name),
                            std::move(args)
                    );
                } else if (auto it = declared_classes.find(method_name); it != end(declared_classes)) {
                    return std::make_unique<Ast::NewInstance>(
                            dynamic_cast<const Runtime::Class &>(*it->second), std::move(args)
                    );
                } else if (method_name == "str") {
                    if (args.size() != 1) {
                        throw ParseError("Function str takes exactly one argument");
                    }
                    return std::make_unique<Ast::Stringify>(std::move(args.front()));
                } else {
                    throw ParseError("Unknown call to " + method_name + "()");
                }
            } else {
                return std::make_unique<Ast::VariableValue>(std::move(names));
            }
        }
    }

    std::vector<std::unique_ptr<Ast::Statement>> ParseTestList() {
        std::vector<std::unique_ptr<Ast::Statement>> result;
        result.push_back(ParseTest());

        while (lexer.CurrentToken() == ',') {
            lexer.NextToken();
            result.push_back(ParseTest());
        }
        return result;
    }

    // Condition -> if LogicalExpr: Suite [else: Suite]
    std::unique_ptr<Ast::Statement> ParseCondition() {
        lexer.Expect<TokenType::If>();
        lexer.NextToken();

        auto condition = ParseTest();

        lexer.Expect<TokenType::Char>(':');
        lexer.NextToken();

        auto if_body = ParseSuite();

        std::unique_ptr<Ast::Statement> else_body;
        if (lexer.CurrentToken().Is<TokenType::Else>()) {
            lexer.ExpectNext<TokenType::Char>(':');
            lexer.NextToken();
            else_body = ParseSuite();
        }

        return std::make_unique<Ast::IfElse>(std::move(condition), std::move(if_body), std::move(else_body));
    }

    // LogicalExpr -> AndTest [OR AndTest]
    // AndTest -> NotTest [AND NotTest]
    // NotTest -> [NOT] NotTest
    //          | Comparison
    std::unique_ptr<Ast::Statement> ParseTest() {
        auto result = ParseAndTest();
        while (lexer.CurrentToken().Is<TokenType::Or>()) {
            lexer.NextToken();
            result = std::make_unique<Ast::Or>(std::move(result), ParseAndTest());
        }
        return result;
    }

    std::unique_ptr<Ast::Statement> ParseAndTest() {
        auto result = ParseNotTest();
        while (lexer.CurrentToken().Is<TokenType::And>()) {
            lexer.NextToken();
            result = std::make_unique<Ast::And>(std::move(result), ParseNotTest());
        }
        return result;
    }

    std::unique_ptr<Ast::Statement> ParseNotTest() {
        if (lexer.CurrentToken().Is<TokenType::Not>()) {
            lexer.NextToken();
            return std::make_unique<Ast::Not>(ParseNotTest());
        } else {
            return ParseComparison();
        }
    }

    // Comparison -> Expr [COMP_OP Expr]
    std::unique_ptr<Ast::Statement> ParseComparison() {
        auto result = ParseExpression();

        const auto tok = lexer.CurrentToken();

        if (tok == '<') {
            lexer.NextToken();
            return std::make_unique<Ast::Comparison>(Runtime::Less, std::move(result), ParseExpression());
        } else if (tok == '>') {
            lexer.NextToken();
            return std::make_unique<Ast::Comparison>(Runtime::Greater, std::move(result), ParseExpression());
        } else if (tok.Is<TokenType::Eq>()) {
            lexer.NextToken();
            return std::make_unique<Ast::Comparison>(Runtime::Equal, std::move(result), ParseExpression());
        } else if (tok.Is<TokenType::NotEq>()) {
            lexer.NextToken();
            return std::make_unique<Ast::Comparison>(Runtime::NotEqual, std::move(result), ParseExpression());
        } else if (tok.Is<TokenType::LessOrEq>()) {
            lexer.NextToken();
            return std::make_unique<Ast::Comparison>(Runtime::LessOrEqual, std::move(result), ParseExpression());
        } else if (tok.Is<TokenType::GreaterOrEq>()) {
            lexer.NextToken();
            return std::make_unique<Ast::Comparison>(Runtime::GreaterOrEqual, std::move(result), ParseExpression());
        } else {
            return result;
        }
    }

    //Statement -> SimpleStatement Newline
    //           | class ClassDefinition
    //           | if Condition
    std::unique_ptr<Ast::Statement> ParseStatement() {
        const auto &tok = lexer.CurrentToken();

        if (tok.Is<TokenType::Class>()) {
            lexer.NextToken();
            return ParseClassDefinition();
        } else if (tok.Is<TokenType::If>()) {
            return ParseCondition();
        } else {
            auto result = ParseSimpleStatement();
            lexer.Expect<TokenType::Newline>();
            lexer.NextToken();
            return result;
        }
    }

    //StatementBody -> return Expression
    //               | print ExpressionList
    //               | AssignmentOrCall
    std::unique_ptr<Ast::Statement> ParseSimpleStatement() {
        const auto &tok = lexer.CurrentToken();

        if (tok.Is<TokenType::Return>()) {
            lexer.NextToken();
            return std::make_unique<Ast::Return>(ParseTest());
        } else if (tok.Is<TokenType::Print>()) {
            lexer.NextToken();
            std::vector<std::unique_ptr<Ast::Statement>> args;
            if (!lexer.CurrentToken().Is<TokenType::Newline>()) {
                args = ParseTestList();
            }
            return std::make_unique<Ast::Print>(std::move(args));
        } else {
            return ParseAssignmentOrCall();
        }
    }
};

std::unique_ptr<Ast::Statement> ParseProgram(Parse::Lexer &lexer) {
    return Parser{lexer}.ParseProgram();
}
