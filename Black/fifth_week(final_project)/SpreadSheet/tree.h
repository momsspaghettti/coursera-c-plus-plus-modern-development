#pragma once

#include <string>
#include <cassert>
#include <memory>
#include <utility>
#include <cstdint>
#include <variant>


class INode {
public:
    [[nodiscard]] virtual double Apply() const = 0;

    [[nodiscard]] virtual std::string toString() const = 0;

    virtual ~INode() = default;
};


class Number : public INode {
public:
    explicit Number(double number) : str_num_(std::to_string(number)) {
        number_ = number;
    }

    explicit Number(const std::string &number);

    [[nodiscard]] double Apply() const override;

    [[nodiscard]] std::string toString() const override;

private:
    const std::string str_num_;
    double number_;
};


class UnaryOperation;


class BinaryOperation : public INode {
public:
    enum class Operation {
        ADD,
        SUB,
        MUL,
        DIV
    };

    explicit BinaryOperation(const std::string &op);

    void SetLeft(const std::shared_ptr<INode> &l) {
        left_ = l;
    }

    void SetRight(const std::shared_ptr<INode> &r) {
        right_ = r;
    }

    [[nodiscard]] double Apply() const override;

    [[nodiscard]] std::string toString() const override;

private:
    Operation operation_;
    std::shared_ptr<INode> left_, right_;
    friend class UnaryOperation;
};


class UnaryOperation : public INode {
public:
    enum class Operation {
        ADD,
        SUB,
    };

    explicit UnaryOperation(const std::string &op);

    void SetArgument(const std::shared_ptr<INode> &arg) {
        argument_ = arg;
    }

    [[nodiscard]] double Apply() const override;

    [[nodiscard]] std::string toString() const override;

private:
    std::shared_ptr<INode> argument_;
    Operation operation_;
};


class Cell : public INode {
public:
    explicit Cell(std::string c) : cell_(std::move(c)){};

    [[nodiscard]] double Apply() const override {
        return 0;
    }

    [[nodiscard]] std::string toString() const override {
        return cell_;
    }

private:
    const std::string cell_;
};


class Root : public INode {
public:
    void SetRoot(const std::shared_ptr<INode> &r) {
        root_ = r;
    }

    [[nodiscard]] double Apply() const override {
        return root_->Apply();
    }

    [[nodiscard]] std::string toString() const override {
        return root_->toString();
    }

private:
    std::shared_ptr<INode> root_;
};