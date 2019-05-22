#include "node.h"


bool EmptyNode::Evaluate(const Date& date_, const string& event_) const
{
    return true;
}


LogicalOperationNode::LogicalOperationNode(const LogicalOperation& logical_operation,
	const shared_ptr<const Node>& left, const shared_ptr<const Node>& right) :
	logical_operation_(logical_operation), left_node_(left), right_node_(right){}


bool LogicalOperationNode::Evaluate(const Date& date_, const string& event_) const
{
    if (logical_operation_ == LogicalOperation::Or)
	{
        return left_node_->Evaluate(date_, event_) || right_node_->Evaluate(date_, event_);
    }

	return left_node_->Evaluate(date_, event_) && right_node_->Evaluate(date_, event_);
}


DateComparisonNode::DateComparisonNode(const Comparison& comparator, 
	const Date& date) : comparator_(comparator), self_date_(date){}


bool DateComparisonNode::Evaluate(const Date& date_, const string& event_) const
{
	if (comparator_ == Comparison::Equal)
		return date_ == self_date_;

    if (comparator_ == Comparison::Greater)
		return date_ > self_date_;

    if (comparator_ == Comparison::GreaterOrEqual)
		return date_ >= self_date_;

    if (comparator_ == Comparison::Less)
		return date_ < self_date_;

    if (comparator_ == Comparison::LessOrEqual)
		return date_ <= self_date_;

	return date_ != self_date_;
}


EventComparisonNode::EventComparisonNode(const Comparison& comparator, 
	const string& event_) : comparator_(comparator), self_event_(event_){}


bool EventComparisonNode::Evaluate(const Date& date_, const string& event_) const
{
	if (comparator_ == Comparison::Equal)
		return event_ == self_event_;

	if (comparator_ == Comparison::Greater)
		return event_ > self_event_;

	if (comparator_ == Comparison::GreaterOrEqual)
		return event_ >= self_event_;

	if (comparator_ == Comparison::Less)
		return event_ < self_event_;

	if (comparator_ == Comparison::LessOrEqual)
		return event_ <= self_event_;

	return event_ != self_event_;
}