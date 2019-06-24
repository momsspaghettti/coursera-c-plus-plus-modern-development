#pragma once
#include <memory>
#include <string>
#include <utility>


// Базовый класс арифметического выражения
class Expression 
{
public:
	virtual ~Expression() = default;

	// Вычисляет значение выражения
	virtual int Evaluate() const = 0;

	// Форматирует выражение как строку
	// Каждый узел берётся в скобки, независимо от приоритета
	virtual std::string ToString() const = 0;
};


using ExpressionPtr = std::unique_ptr<Expression>;


namespace Expr
{
    class Value : public Expression
    {
    public:
        Value(const int& value) : value_(value) {}

        int Evaluate() const override;

        std::string ToString() const override;

    private:
        int value_;
    };


    class Sum : public Expression
    {
    public:
        Sum(ExpressionPtr left, ExpressionPtr right) : left_ptr_(std::move(left)), 
                                                       right_ptr_(std::move(right)) {}

        int Evaluate() const override;

        std::string ToString() const override;

    private:
        ExpressionPtr left_ptr_, right_ptr_;
    };


    class Product : public Expression
    {
    public:
        Product(ExpressionPtr left, ExpressionPtr right) : left_ptr_(std::move(left)),
                                                           right_ptr_(std::move(right)) {}

        int Evaluate() const override;

        std::string ToString() const override;

    private:
        ExpressionPtr left_ptr_, right_ptr_;
    };
}


// Функции для формирования выражения
ExpressionPtr Value(int value);
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right);
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right);


void TestCommon();