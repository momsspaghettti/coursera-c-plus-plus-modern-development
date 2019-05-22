#include "test_node.h"
#include "node.h"
#include "UnitTestsFramework.h"


using namespace std;


void TestDateComparisonNode() {
	const Date november_1{ 2017, 11, 1 };
	const Date november_2{ 2017, 11, 2 };
	const Date november_3{ 2017, 11, 3 };

	DateComparisonNode less(Comparison::Less, november_2);
	Assert(less.Evaluate(november_1, ""), "DateComparisonNode: less 1");
	Assert(!less.Evaluate(november_3, ""), "DateComparisonNode: less 2");

	DateComparisonNode not_equal(Comparison::NotEqual, november_2);
	Assert(not_equal.Evaluate(november_1, ""), "DateComparisonNode: not_equal 1");
	Assert(!not_equal.Evaluate(november_2, ""), "DateComparisonNode: not_equal 2");

	DateComparisonNode greater_eq(Comparison::GreaterOrEqual, november_2);
	Assert(greater_eq.Evaluate(november_2, ""), "DateComparisonNode: greater_eq 1");
	Assert(greater_eq.Evaluate(november_3, ""), "DateComparisonNode: greater_eq 2");
	Assert(!greater_eq.Evaluate(november_1, ""), "DateComparisonNode: greater_eq 3");
}

void TestEventComparisonNode() {
	const Date january_1{ 1970, 1, 1 };

	EventComparisonNode less_eq(Comparison::LessOrEqual, "b");
	Assert(less_eq.Evaluate(january_1, "a"), "EventComparisonNode: less_eq 1");
	Assert(!less_eq.Evaluate(january_1, "c"), "EventComparisonNode: less_eq 2");
	Assert(less_eq.Evaluate(january_1, "b"), "EventComparisonNode: less_eq 3");

	EventComparisonNode equal(Comparison::Equal, "b");
	Assert(equal.Evaluate(january_1, "b"), "EventComparisonNode: equal 1");
	Assert(!equal.Evaluate(january_1, "a"), "EventComparisonNode: equal 2");
	Assert(!equal.Evaluate(january_1, "c"), "EventComparisonNode: equal 3");

	EventComparisonNode greater(Comparison::Greater, "b");
	Assert(greater.Evaluate(january_1, "c"), "EventComparisonNode: greater 1");
	Assert(!greater.Evaluate(january_1, "b"), "EventComparisonNode: greater 2");
	Assert(!greater.Evaluate(january_1, "a"), "EventComparisonNode: greater 3");
}

struct FalseNode : Node {
	bool Evaluate(const Date&, const string&) const override {
		return false;
	};
};

void TestLogicalOperationNode() {
	{
		map<bool, shared_ptr<Node>> bool_to_node = {
			{false, make_shared<FalseNode>()},
			{true, make_shared<EmptyNode>()}
		};

		for (bool value_one : {false, true}) {
			for (bool value_two : {false, true}) {
				LogicalOperationNode or_node(
					LogicalOperation::Or, bool_to_node[value_one], bool_to_node[value_two]
				);
				LogicalOperationNode and_node(
					LogicalOperation::And, bool_to_node[value_one], bool_to_node[value_two]
				);

				AssertEqual(or_node.Evaluate({}, ""), value_one || value_two, "LogicalOperationNode Or");
				AssertEqual(and_node.Evaluate({}, ""), value_one && value_two, "LogicalOperationNode And");
			}
		}
	}
	{
		LogicalOperationNode root(
			LogicalOperation::And,
			make_shared<DateComparisonNode>(Comparison::Equal, Date{ 2017, 11, 18 }),
			make_shared<EventComparisonNode>(Comparison::Equal, "Saturday")
		);

		Assert(root.Evaluate({ 2017, 11, 18 }, "Saturday"), "LogicalOperationNode propagates arguments 1");
		Assert(!root.Evaluate({ 2017, 11, 18 }, "Friday"), "LogicalOperationNode propagates arguments 2");
		Assert(!root.Evaluate({ 2017, 11, 1 }, "Saturday"), "LogicalOperationNode propagates arguments 1");
	}
}