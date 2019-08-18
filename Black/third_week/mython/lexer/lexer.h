#pragma once
#include <iosfwd>
#include <string>
#include <sstream>
#include <variant>
#include <stdexcept>
#include <optional>
#include <unordered_map>


class TestRunner;

namespace Parse {

    namespace TokenType {
        struct Number { //
            int value;
        };

        struct Id {
            std::string value;
        };

        struct Char { //
            char value;
        };

        struct String { //
            std::string value;
        };

        struct Class{};
        struct Return{};
        struct If {};
        struct Else {};
        struct Def {};
        struct Newline {}; //
        struct Print {};
        struct Indent {}; //
        struct Dedent {}; //
        struct Eof {}; //
        struct And {};
        struct Or {};
        struct Not {};
        struct Eq {};
        struct NotEq {};
        struct LessOrEq {};
        struct GreaterOrEq {};
        struct None {};
        struct True {};
        struct False {};
    }

    using TokenBase = std::variant<
            TokenType::Number,
            TokenType::Id,
            TokenType::Char,
            TokenType::String,
            TokenType::Class,
            TokenType::Return,
            TokenType::If,
            TokenType::Else,
            TokenType::Def,
            TokenType::Newline,
            TokenType::Print,
            TokenType::Indent,
            TokenType::Dedent,
            TokenType::And,
            TokenType::Or,
            TokenType::Not,
            TokenType::Eq,
            TokenType::NotEq,
            TokenType::LessOrEq,
            TokenType::GreaterOrEq,
            TokenType::None,
            TokenType::True,
            TokenType::False,
            TokenType::Eof
    >;


//По сравнению с условием задачи мы добавили в тип Token несколько
//удобных методов, которые делают код короче. Например,
//
//token.Is<TokenType::Id>()
//
//гораздо короче, чем
//
//std::holds_alternative<TokenType::Id>(token).
    struct Token : TokenBase {
        using TokenBase::TokenBase;

        template <typename T>
        [[nodiscard]] bool Is() const {
            return std::holds_alternative<T>(*this);
        }

        template <typename T>
        const T& As() const {
            return std::get<T>(*this);
        }

        template <typename T>
        const T* TryAs() const {
            return std::get_if<T>(this);
        }
    };

    bool operator == (const Token& lhs, const Token& rhs);
    bool operator != (const Token& lhs, const Token& rhs);

    std::ostream& operator << (std::ostream& os, const Token& rhs);

    class LexerError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class Lexer {
    public:
        explicit Lexer(std::istream& input);

        [[nodiscard]] const Token& CurrentToken() const;

        Token NextToken();

        template <typename T>
        const T& Expect() const {
            if (CurrentToken().Is<T>()) {
                return CurrentToken().As<T>();
            }

            throw LexerError("Expectation Error!");
        }

        template <typename T, typename U>
        void Expect(const U& value) const {
            try {
                if (CurrentToken().As<T>().value != value) {
                    throw LexerError("Expectation Error!");
                }
            } catch (std::bad_variant_access&) {
                throw LexerError("Expectation Error!");
            }
        }

        template <typename T>
        const T& ExpectNext() {
            NextToken();
            return Expect<T>();
        }

        template <typename T, typename U>
        void ExpectNext(const U& value) {
            NextToken();
            Expect<T>(value);
        }

    private:
        class IndentController {
        public:
            IndentController();

            void indentNext() {
                indent_next_ = true;
            }

            void notIndentNext() {
                indent_next_ = false;
            }

            [[nodiscard]] bool isIndentNext() const {
                return indent_next_;
            }

            [[nodiscard]] bool needNewLine() const {
                return need_new_line_;
            }

            void setEof() {
                need_new_line_ = false;
            }

            void setNewLine() {
                need_new_line_ = false;
            }

            [[nodiscard]] std::optional<Token> updateIndent(std::istream&);

        private:
            bool indent_next_;
            unsigned current_indents_;

            unsigned needed_indents_;
            unsigned needed_dedents_;
            bool need_new_line_ = true;

            void update_next() {
                if (needed_dedents_ == 0 && needed_indents_ == 0)
                    indent_next_ = false;
            }
        };

        std::istream& in_;
        Token current_token_;

        IndentController indentController;

        [[nodiscard]] Token readToken();
        [[nodiscard]] TokenType::Number readNumber();
        [[nodiscard]] TokenType::String readString();
        [[nodiscard]] std::string readStringToEnd(const char& end);
        [[nodiscard]] Token readIdOrDeclaredToken();
        [[nodiscard]] bool isBoolToken(const char& c) const;
        [[nodiscard]] Token readBoolToken();

        const std::unordered_map<std::string, Token> declared_tokens_ = {
                {"class", TokenType::Class{}},
                {"return", TokenType::Return{}},
                {"if", TokenType::If{}},
                {"else", TokenType::Else{}},
                {"def", TokenType::Def{}},
                {"print", TokenType::Print{}},
                {"and", TokenType::And{}},
                {"or", TokenType::Or{}},
                {"not", TokenType::Not{}},
                {"None", TokenType::None{}},
                {"True", TokenType::True{}},
                {"False", TokenType::False{}},
        };

        const std::unordered_map<std::string, Token> bool_tokens_ = {
                {"==", TokenType::Eq{}},
                {"!=", TokenType::NotEq{}},
                {"<=", TokenType::LessOrEq{}},
                {">=", TokenType::GreaterOrEq{}},
        };

        [[nodiscard]] bool is_char_token(const char& c) const;
    };

    void RunLexerTests(TestRunner& test_runner);

} /* namespace Parse */


void TestLexer();