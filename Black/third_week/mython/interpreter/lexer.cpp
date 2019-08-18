#include "lexer.h"
#include <charconv>
#include <iostream>


namespace Parse {

    bool operator==(const Token &lhs, const Token &rhs) {
        using namespace TokenType;

        if (lhs.index() != rhs.index()) {
            return false;
        }
        if (lhs.Is<Char>()) {
            return lhs.As<Char>().value == rhs.As<Char>().value;
        } else if (lhs.Is<Number>()) {
            return lhs.As<Number>().value == rhs.As<Number>().value;
        } else if (lhs.Is<String>()) {
            return lhs.As<String>().value == rhs.As<String>().value;
        } else if (lhs.Is<Id>()) {
            return lhs.As<Id>().value == rhs.As<Id>().value;
        } else {
            return true;
        }
    }

    bool operator!=(const Token &lhs, const Token &rhs) {
        return !(lhs == rhs);
    }

    std::ostream &operator<<(std::ostream &os, const Token &rhs) {
        using namespace TokenType;

#define VALUED_OUTPUT(type) \
  if (auto p = rhs.TryAs<type>()) return os << #type << '{' << p->value << '}';

        VALUED_OUTPUT(Number)
        VALUED_OUTPUT(Id)
        VALUED_OUTPUT(String)
        VALUED_OUTPUT(Char)

#undef VALUED_OUTPUT

#define UNVALUED_OUTPUT(type) \
    if (rhs.Is<type>()) return os << #type;

        UNVALUED_OUTPUT(Class)
        UNVALUED_OUTPUT(Return)
        UNVALUED_OUTPUT(If)
        UNVALUED_OUTPUT(Else)
        UNVALUED_OUTPUT(Def)
        UNVALUED_OUTPUT(Newline)
        UNVALUED_OUTPUT(Print)
        UNVALUED_OUTPUT(Indent)
        UNVALUED_OUTPUT(Dedent)
        UNVALUED_OUTPUT(And)
        UNVALUED_OUTPUT(Or)
        UNVALUED_OUTPUT(Not)
        UNVALUED_OUTPUT(Eq)
        UNVALUED_OUTPUT(NotEq)
        UNVALUED_OUTPUT(LessOrEq)
        UNVALUED_OUTPUT(GreaterOrEq)
        UNVALUED_OUTPUT(None)
        UNVALUED_OUTPUT(True)
        UNVALUED_OUTPUT(False)
        UNVALUED_OUTPUT(Eof)

#undef UNVALUED_OUTPUT

        return os << "Unknown token :(";
    }


    Lexer::IndentController::IndentController() {
        indent_next_ = true;
        current_indents_ = 0;

        needed_indents_ = 0;
        needed_dedents_ = 0;

        need_new_line_ = true;
    }


    Lexer::Lexer(std::istream &input) : in_(input) {
        current_token_ = readToken();
        indentController.notIndentNext();
    }

    const Token &Lexer::CurrentToken() const {
        return current_token_;
    }

    Token Lexer::NextToken() {
        current_token_ = readToken();
        return CurrentToken();
    }

    Token Lexer::readToken() {
        char c = in_.get();

        if (indentController.isIndentNext()) {
            in_.putback(c);
            const auto result = indentController.updateIndent(in_);
            if (result)
                return *result;
            return readToken();
        }

        if (c == -1) {
            if (indentController.needNewLine()) {
                indentController.setEof();
                return TokenType::Newline{};
            }
            return TokenType::Eof{};
        }

        if (c == '\n') {
            indentController.indentNext();
            indentController.setNewLine();
            return TokenType::Newline{};
        }

        if (std::isdigit(c)) {
            in_.putback(c);
            return readNumber();
        }

        if (is_char_token(c)) {
            return TokenType::Char{c};
        }

        if (c == '\'' || c == '\"') {
            in_.putback(c);
            return readString();
        }

        if (c == ' ') {
            return readToken();
        }

        in_.putback(c);
        return readIdOrDeclaredToken();
    }

    TokenType::Number Lexer::readNumber() {
        int result = 0;

        while (std::isdigit(in_.peek())) {
            result *= 10;
            result += in_.get() - '0';
        }

        return TokenType::Number{result};
    }

    bool Lexer::is_char_token(const char &c) const {
        return
                (c == '+' || c == '-' ||
                 c == '.' || c == ',' ||
                 c == '=' ||
                 c == '>' || c == '<' ||
                 c == '*' || c == '/' ||
                 c == '%' ||
                 c == '(' || c == ')' ||
                 c == '[' || c == ']' ||
                 c == '{' || c == '}' ||
                 c == ':' || c == '?' ||
                 c == '!') && !isBoolToken(c);
    }

    bool Lexer::isBoolToken(const char &c) const {
        return
                (c == '!' && in_.peek() == '=') ||
                (c == '=' && in_.peek() == '=') ||
                (c == '<' && in_.peek() == '=') ||
                (c == '>' && in_.peek() == '=');
    }

    TokenType::String Lexer::readString() {
        return TokenType::String{readStringToEnd(in_.get())};
    }

    std::string Lexer::readStringToEnd(const char &end) {
        std::string result;
        std::getline(in_, result, end);

        return result;
    }

    Token Lexer::readIdOrDeclaredToken() {
        char c = in_.get();
        if (isBoolToken(c)) {
            in_.putback(c);
            return readBoolToken();
        }
        in_.putback(c);

        std::string tmp;
        while (
                in_.peek() != ' ' && in_.peek() != '\n' &&
                in_.peek() != -1 && !is_char_token(in_.peek())
                ) {
            tmp += in_.get();
        }

        const auto finder = declared_tokens_.find(tmp);
        if (finder != declared_tokens_.end()) {
            return finder->second;
        }

        return TokenType::Id{tmp};
    }

    Token Lexer::readBoolToken() {
        std::string tmp;
        tmp += in_.get();
        tmp += in_.get();

        return bool_tokens_.at(tmp);
    }

    std::optional<Token> Lexer::IndentController::updateIndent(std::istream &in) {
        if (needed_indents_ > 0) {
            --needed_indents_;
            ++current_indents_;
            update_next();
            return TokenType::Indent{};
        }

        if (needed_dedents_ > 0) {
            --needed_dedents_;
            --current_indents_;
            update_next();
            return TokenType::Dedent{};
        }

        unsigned spaces = 0;
        while (in.peek() == ' ') {
            in.get();
            ++spaces;
        }

        if (in.peek() == '\n') {
            in.get();
            return std::nullopt;
        } else if (in.peek() != -1) {
            need_new_line_ = true;
        } else if (in.peek() == -1) {
            need_new_line_ = false;
        }

        spaces = spaces % 2 == 0 ? spaces / 2 : spaces / 2 + 1;

        if (spaces == current_indents_) {
            indent_next_ = false;
            return std::nullopt;
        } else if (spaces > current_indents_) {
            needed_indents_ = spaces - current_indents_;
            --needed_indents_;
            ++current_indents_;
            update_next();
            return TokenType::Indent{};
        } else {
            needed_dedents_ = current_indents_ - spaces;
            --needed_dedents_;
            --current_indents_;
            update_next();
            return TokenType::Dedent{};
        }
    }

} /* namespace Parse */