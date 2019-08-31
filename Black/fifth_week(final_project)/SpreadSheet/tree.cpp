#include "tree.h"


Number::Number(const std::string &number) : str_num_(number) {
    number_ = 0;
    size_t pos = 0;
    while (pos < number.size()) {
        const char ch = number[pos];
        if (std::isdigit(ch)) {
            number_ *= 10;
            number_ += ch - '0';
        } else {
            assert(ch == '.');
            break;
        }
        ++pos;
    }

    if (pos < number.size()) {
        assert(number[pos] == '.');
        ++pos;
        uint64_t pow = 1;
        while (pos < number.size()) {
            const char ch = number[pos];
            assert(std::isdigit(ch));
            pow *= 10;
            number_ += (static_cast<double>(ch - '0') / pow);
            ++pos;
        }
    }
}

double Number::Apply() const {
    return number_;
}

std::string Number::toString() const {
    return str_num_;
}


BinaryOperation::BinaryOperation(const std::string &op) {
    if (op == "+") {
        operation_ = Operation::ADD;
    } else if (op == "-") {
        operation_ = Operation::SUB;
    } else if (op == "*") {
        operation_ = Operation::MUL;
    } else {
        assert(op == "/");
        operation_ = Operation::DIV;
    }
}

double BinaryOperation::Apply() const {
    switch (operation_) {
        case Operation::ADD:
            return left_->Apply() + right_->Apply();
        case Operation::SUB:
            return left_->Apply() - right_->Apply();
        case Operation::MUL:
            return left_->Apply() * right_->Apply();
        case Operation::DIV:
            return left_->Apply() / right_->Apply();
    }
}

std::string BinaryOperation::toString() const {
    std::string arg1_str = left_->toString();
    std::string arg2_str = right_->toString();

    if (operation_ != Operation::ADD) {
        const auto arg1 = dynamic_cast<BinaryOperation *>(left_.get());
        const auto arg2 = dynamic_cast<BinaryOperation *>(right_.get());
        if (operation_ == Operation::SUB) {
            if (arg2) {
                if (arg2->operation_ == Operation::ADD ||
                    arg2->operation_ == Operation::SUB) {
                    arg2_str = "(" + arg2_str + ")";
                }
            }
        }

        if (operation_ == Operation::MUL) {
            if (arg1) {
                if (arg1->operation_ == Operation::ADD ||
                    arg1->operation_ == Operation::SUB) {
                    arg1_str = "(" + arg1_str + ")";
                }
            }
            if (arg2) {
                if (arg2->operation_ == Operation::ADD ||
                    arg2->operation_ == Operation::SUB) {
                    arg2_str = "(" + arg2_str + ")";
                }
            }
        }

        if (operation_ == Operation::DIV) {
            if (arg1) {
                if (arg1->operation_ == Operation::ADD ||
                    arg1->operation_ == Operation::SUB) {
                    arg1_str = "(" + arg1_str + ")";
                }
            }
            if (arg2) {
                arg2_str = "(" + arg2_str + ")";
            }
        }
    }

    switch (operation_) {
        case Operation::ADD:
            return arg1_str + "+" + arg2_str;
        case Operation::SUB:
            return arg1_str + "-" + arg2_str;
        case Operation::MUL:
            return arg1_str + "*" + arg2_str;
        case Operation::DIV:
            return arg1_str + "/" + arg2_str;
    }
}


UnaryOperation::UnaryOperation(const std::string &op) {
    if (op == "-") {
        operation_ = Operation::SUB;
    } else {
        assert(op == "+");
        operation_ = Operation::ADD;
    }
}

double UnaryOperation::Apply() const {
    if (operation_ == Operation::SUB) {
        return -1 * argument_->Apply();
    }
    return argument_->Apply();
}

std::string UnaryOperation::toString() const {
    std::string arg_str = argument_->toString();
    const auto arg = dynamic_cast<BinaryOperation *>(argument_.get());
    if (arg) {
        if (arg->operation_ == BinaryOperation::Operation::ADD ||
            arg->operation_ == BinaryOperation::Operation::SUB) {
            arg_str = "(" + arg_str + ")";
        }
    }
    if (operation_ == Operation::SUB) {
        return "-" + arg_str;
    }
    return "+" + arg_str;
}