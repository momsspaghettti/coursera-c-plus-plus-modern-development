#include "tasks_tracker.h"
#include "UnitTestsFramework.h"
#include <algorithm>


void TeamTasks::remove_empty(TasksInfo& tasks)
{
	TasksInfo tmp;
    for (auto pair_ : tasks)
    {
		if (pair_.second != 0)
			tmp.insert(pair_);
    }

	tasks = std::move(tmp);
}


const TasksInfo& TeamTasks::GetPersonTasksInfo(const std::string& person) const
{
	return programmers_.at(person);
}


void TeamTasks::AddNewTask(const std::string& person)
{
	++programmers_[person][TaskStatus::NEW];
}


std::tuple<TasksInfo, TasksInfo> TeamTasks::PerformPersonTasks(const std::string& person, int task_count)
{
	TasksInfo updated_tasks;
	
    TasksInfo& current_tasks = programmers_[person];

    for (int i = 0; i < 3; ++i)
    {
        if(task_count == 0)
            break;

		const auto untouched_status = static_cast<TaskStatus>(i);
		const auto updated_status = static_cast<TaskStatus>(i + 1);
		const int tasks_count = current_tasks[untouched_status];
        
		updated_tasks[updated_status] += std::min(tasks_count, task_count);
		current_tasks[untouched_status] -= std::min(tasks_count, task_count);
        task_count -= std::min(tasks_count, task_count);
    }

	remove_empty(updated_tasks);

	TasksInfo untouched_tasks = current_tasks;
	untouched_tasks.erase(TaskStatus::DONE);
	remove_empty(untouched_tasks);
	
	current_tasks = current_tasks + updated_tasks;
	remove_empty(current_tasks);

	return std::make_tuple(std::move(updated_tasks), 
		std::move(untouched_tasks));
}


TasksInfo operator+(const TasksInfo& lhs, const TasksInfo& rhs)
{
	TasksInfo result;

	for (int i = 0; i < 4; ++i)
	{
		const auto status = static_cast<TaskStatus>(i);

		auto lhs_finder = lhs.find(status);
		auto rhs_finder = rhs.find(status);

        if (lhs_finder != lhs.end())
        {
			result[status] = lhs_finder->second;

			if (rhs_finder != rhs.end())
				result[status] += rhs_finder->second;
        }
        else if (rhs_finder != rhs.end())
        {
			result[status] = rhs_finder->second;
        }
	}

	return result;
}


std::ostream& operator<<(std::ostream& out, const TaskStatus& status)
{
	switch (status)
	{
	case TaskStatus::NEW:
		out << "NEW";
		break;

	case TaskStatus::IN_PROGRESS:
		out << "IN_PROGRESS";
		break;

	case TaskStatus::TESTING:
		out << "TESTING";
		break;

	case TaskStatus::DONE:
		out << "DONE";
		break;
	}

	return out;
}


void TestTeamTasks()
{
	TeamTasks tasks;

	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) 
	{
		tasks.AddNewTask("Ivan");
	}

	{
		const auto ilia_tasks = tasks.GetPersonTasksInfo("Ilia");
		const TasksInfo ilia_expected = { {TaskStatus::NEW, 1} };
		ASSERT_EQUAL(ilia_tasks, ilia_expected);
	}

	{
		const auto ivan_tasks = tasks.GetPersonTasksInfo("Ivan");
		const TasksInfo ivan_expected = { {TaskStatus::NEW, 3} };
		ASSERT_EQUAL(ivan_tasks, ivan_expected);
	}

	{
	    const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Ivan", 2);
        
	    const TasksInfo expected_updated_tasks = {{TaskStatus::IN_PROGRESS, 2}};
		const TasksInfo expected_untouched_tasks = { {TaskStatus::NEW, 1} };

		ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
		ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
	}

	{
		const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Ivan", 2);

		const TasksInfo expected_updated_tasks = { {TaskStatus::IN_PROGRESS, 1}, {TaskStatus::TESTING, 1} };
		const TasksInfo expected_untouched_tasks = { {TaskStatus::IN_PROGRESS, 1} };

		ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
		ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
	}

	{
		for (int i = 0; i < 5; ++i)
		{
			tasks.AddNewTask("Alice");
		}

		{
			const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Alice", 5);

			const TasksInfo expected_updated_tasks = { {TaskStatus::IN_PROGRESS, 5} };
			const TasksInfo expected_untouched_tasks = { };

			ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
			ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
		}

		{
			const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Alice", 5);

			const TasksInfo expected_updated_tasks = { {TaskStatus::TESTING, 5} };
			const TasksInfo expected_untouched_tasks = { };

			ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
			ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
		}

		{
			const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Alice", 1);

			const TasksInfo expected_updated_tasks = { {TaskStatus::DONE, 1} };
			const TasksInfo expected_untouched_tasks = { {TaskStatus::TESTING, 4} };

			ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
			ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
		}

		for (int i = 0; i < 5; ++i)
		{
			tasks.AddNewTask("Alice");
		}

		{
			const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Alice", 2);

			const TasksInfo expected_updated_tasks = { {TaskStatus::IN_PROGRESS, 2} };
			const TasksInfo expected_untouched_tasks = { {TaskStatus::NEW, 3}, {TaskStatus::TESTING, 4} };

			ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
			ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
		}

		{
			const auto alice_tasks = tasks.GetPersonTasksInfo("Alice");
			const TasksInfo alice_expected = { {TaskStatus::NEW, 3}, {TaskStatus::IN_PROGRESS, 2}, 
			    {TaskStatus::TESTING, 4}, {TaskStatus::DONE, 1} };
			ASSERT_EQUAL(alice_tasks, alice_expected);
		}

		{
			const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Alice", 4);

			const TasksInfo expected_updated_tasks = { {TaskStatus::IN_PROGRESS, 3}, {TaskStatus::TESTING, 1} };
			const TasksInfo expected_untouched_tasks = { {TaskStatus::IN_PROGRESS, 1}, {TaskStatus::TESTING, 4} };

			ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
			ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
		}

		{
			const auto alice_tasks = tasks.GetPersonTasksInfo("Alice");
			const TasksInfo alice_expected = { {TaskStatus::IN_PROGRESS, 4},
				{TaskStatus::TESTING, 5}, {TaskStatus::DONE, 1} };
			ASSERT_EQUAL(alice_tasks, alice_expected);
		}
	}
}


class TeamTasksTester
{
public:
    static void OneSimpleTest()
    {
		TeamTasks tasks;

		const TasksInfo input = { {TaskStatus::NEW, 3}, {TaskStatus::IN_PROGRESS, 2}, 
		    {TaskStatus::TESTING, 4}, {TaskStatus::DONE, 1} };
		tasks.programmers_.insert({ "Tester", input });

		const auto [updated_tasks, untouched_tasks] = tasks.PerformPersonTasks("Tester", 4);

		const TasksInfo expected_updated_tasks = { {TaskStatus::IN_PROGRESS, 3}, {TaskStatus::TESTING, 1} };
		const TasksInfo expected_untouched_tasks = { {TaskStatus::IN_PROGRESS, 1}, {TaskStatus::TESTING, 4} };

		ASSERT_EQUAL(updated_tasks, expected_updated_tasks);
		ASSERT_EQUAL(untouched_tasks, expected_untouched_tasks);
    }
};


void TestTasksTracker()
{
	TestRunner tr;

	RUN_TEST(tr, TestTeamTasks);
	RUN_TEST(tr, TeamTasksTester::OneSimpleTest);
}