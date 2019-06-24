#include "Common.h"


namespace Expr
{
    int Value::Evaluate() const
    {
        return value_;
    }


    std::string Value::ToString() const
    {
        return std::to_string(value_);
    }


    int Sum::Evaluate() const
    {
        return left_ptr_->Evaluate() + right_ptr_->Evaluate();
    }


    std::string Sum::ToString() const
    {
        return "(" + left_ptr_->ToString() + ")" +
			"+" + "(" + right_ptr_->ToString() + ")";
    }


    int Product::Evaluate() const
    {
        return left_ptr_->Evaluate() * right_ptr_->Evaluate();
    }


    std::string Product::ToString() const
    {
		return "(" + left_ptr_->ToString() + ")" +
			"*" + "(" + right_ptr_->ToString() + ")";
    }
}


std::unique_ptr<Expression> Value(int value)
{
	return std::make_unique<Expr::Value>(value);
}


ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right)
{
	return std::make_unique<Expr::Sum>(std::move(left), std::move(right));
}


ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right)
{
	return std::make_unique<Expr::Product>(std::move(left), std::move(right));
}