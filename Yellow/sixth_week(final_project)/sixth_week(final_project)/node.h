#pragma once
#include "date.h"
#include <string>
#include <memory>


enum class LogicalOperation
{
    And,
    Or,
};


enum class Comparison
{
	Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};


class Node
{
public:
	virtual bool Evaluate(const Date& date_, 
		const string& event_) const = 0;
};


class EmptyNode : public Node
{
public:
    bool Evaluate(const Date& date_, const string& event_) const override;
};


class LogicalOperationNode : public Node
{
public:
	LogicalOperationNode(const LogicalOperation& logical_operation, 
		const shared_ptr<const Node>& left, const shared_ptr<const Node>& right);

    bool Evaluate(const Date& date_, const string& event_) const override;

private:
	LogicalOperation logical_operation_;
	shared_ptr<const Node> left_node_, right_node_;
};


class DateComparisonNode : public Node
{
public:
	DateComparisonNode(const Comparison& comparator, const Date& date);

    bool Evaluate(const Date& date_, const string& event_) const override;

private:
	const Comparison comparator_;
	const Date self_date_;
};


class EventComparisonNode : public Node
{
public:
	EventComparisonNode(const Comparison& comparator, const string& event_);

    bool Evaluate(const Date& date_, const string& event_) const override;

private:
	const Comparison comparator_;
	const string self_event_;
};