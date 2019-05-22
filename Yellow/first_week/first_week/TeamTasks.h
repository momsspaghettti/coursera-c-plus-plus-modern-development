#pragma once
#include <tuple>
#include <map>
#include <string>
#include <iostream>


using namespace std;


enum class TaskStatus {
	NEW,          // нова€
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};


inline ostream& operator<<(ostream& stream, const TaskStatus& task_status)
{
	switch (static_cast<int>(task_status))
	{
	case 0:
		stream << "NEW:";
		break;
	case 1:
		stream << "IN_PROGRESS:";
		break;
	case 2:
		stream << "TESTING:";
		break;
	case 3:
		stream << "DONE:";
		break;
	default:
		stream << "Error!";
		break;
	}

	return stream;
}


using TasksInfo = map<TaskStatus, int>;


#pragma once
class TeamTasks
{
public:
	// ѕолучить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const
	{
		return programmers_.at(person);
	}

	// ƒобавить новую задачу (в статусе NEW) дл€ конкретного разработчитка
	void AddNewTask(const string& person)
	{
		programmers_[person][TaskStatus::NEW] += 1;

		if (programmers_.at(person).at(TaskStatus::NEW) == 1)
		{
			programmers_[person][TaskStatus::IN_PROGRESS] += 0;
			programmers_[person][TaskStatus::TESTING] += 0;
			programmers_[person][TaskStatus::DONE] += 0;
		}
	}

	// ќбновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count)
	{
		TasksInfo updated_tasks, untouched_tasks;

		if (programmers_.count(person) == 0)
		{
			return make_tuple(updated_tasks, untouched_tasks);
		}

		if (programmers_.at(person).at(TaskStatus::NEW) > task_count)
		{
			programmers_[person][TaskStatus::NEW] -= task_count;
			untouched_tasks[TaskStatus::NEW] = programmers_.at(person).at(TaskStatus::NEW);

			updated_tasks[TaskStatus::IN_PROGRESS] = task_count;
			untouched_tasks[TaskStatus::IN_PROGRESS] = programmers_.at(person).at(TaskStatus::IN_PROGRESS);
			programmers_[person][TaskStatus::IN_PROGRESS] += task_count;

			if (programmers_.at(person).at(TaskStatus::TESTING) > 0)
			{
				untouched_tasks[TaskStatus::TESTING] = programmers_.at(person).at(TaskStatus::TESTING);
			}

			if (programmers_.at(person).at(TaskStatus::DONE) > 0)
			{
				untouched_tasks[TaskStatus::DONE] = programmers_.at(person).at(TaskStatus::DONE);
			}
		}
		else
		{
			task_count -= programmers_.at(person).at(TaskStatus::NEW);
			updated_tasks[TaskStatus::IN_PROGRESS] = programmers_.at(person).at(TaskStatus::NEW);

			if (programmers_.at(person).at(TaskStatus::IN_PROGRESS) > task_count)
			{
				programmers_[person][TaskStatus::IN_PROGRESS] -= task_count;
				untouched_tasks[TaskStatus::IN_PROGRESS] = programmers_.at(person).at(TaskStatus::IN_PROGRESS);

				updated_tasks[TaskStatus::TESTING] = task_count;
				untouched_tasks[TaskStatus::TESTING] = programmers_.at(person).at(TaskStatus::TESTING);
				programmers_[person][TaskStatus::TESTING] += task_count;

				if (programmers_.at(person).at(TaskStatus::DONE) > 0)
				{
					untouched_tasks[TaskStatus::DONE] = programmers_.at(person).at(TaskStatus::DONE);
				}
			}
			else
			{
				task_count -= programmers_.at(person).at(TaskStatus::IN_PROGRESS);
				updated_tasks[TaskStatus::TESTING] = programmers_.at(person).at(TaskStatus::IN_PROGRESS);

				if (programmers_.at(person).at(TaskStatus::TESTING) > task_count)
				{
					programmers_[person][TaskStatus::TESTING] -= task_count;
					untouched_tasks[TaskStatus::TESTING] = programmers_.at(person).at(TaskStatus::TESTING);

					updated_tasks[TaskStatus::DONE] = task_count;
					untouched_tasks[TaskStatus::DONE] = programmers_.at(person).at(TaskStatus::DONE);
					programmers_[person][TaskStatus::DONE] += task_count;
				}
				else
				{
					untouched_tasks[TaskStatus::DONE] = programmers_.at(person).at(TaskStatus::DONE);
					updated_tasks[TaskStatus::DONE] = programmers_.at(person).at(TaskStatus::TESTING);
					programmers_[person][TaskStatus::DONE] += programmers_.at(person).at(TaskStatus::TESTING);
					programmers_[person][TaskStatus::TESTING] = 0;
				}

				programmers_[person][TaskStatus::TESTING] += programmers_.at(person).at(TaskStatus::IN_PROGRESS);
				programmers_[person][TaskStatus::IN_PROGRESS] = 0;
			}

			programmers_[person][TaskStatus::IN_PROGRESS] += programmers_.at(person).at(TaskStatus::NEW);
			programmers_[person][TaskStatus::NEW] = 0;
		}

		remove_empty(updated_tasks, untouched_tasks);

		return make_tuple(updated_tasks, untouched_tasks);
	}

private:
	map<string, TasksInfo> programmers_;

	static void remove_empty(TasksInfo& updated_tasks, TasksInfo& untouched_tasks)
	{
		if (untouched_tasks[TaskStatus::NEW] == 0)
		{
			untouched_tasks.erase(TaskStatus::NEW);
		}

		if (untouched_tasks[TaskStatus::IN_PROGRESS] == 0)
		{
			untouched_tasks.erase(TaskStatus::IN_PROGRESS);
		}

		if (untouched_tasks[TaskStatus::TESTING] == 0)
		{
			untouched_tasks.erase(TaskStatus::TESTING);
		}

		untouched_tasks[TaskStatus::DONE] += 0;
		untouched_tasks.erase(TaskStatus::DONE);

		updated_tasks[TaskStatus::NEW] += 0;
		updated_tasks.erase(TaskStatus::NEW);

		if (updated_tasks[TaskStatus::IN_PROGRESS] == 0)
		{
			updated_tasks.erase(TaskStatus::IN_PROGRESS);
		}

		if (updated_tasks[TaskStatus::TESTING] == 0)
		{
			updated_tasks.erase(TaskStatus::TESTING);
		}

		if (updated_tasks[TaskStatus::DONE] == 0)
		{
			updated_tasks.erase(TaskStatus::DONE);
		}
	}
};


inline void PrintTasksInfo(TasksInfo tasks_info) {
	for (const auto& item : tasks_info)
	{
		cout << item.first << " " << item.second << " ";
	}
}


inline void PrintTasksInfo(const TasksInfo& updated_tasks, const TasksInfo& untouched_tasks) {
	std::cout << "Updated: [";
	PrintTasksInfo(updated_tasks);
	std::cout << "] ";

	std::cout << "Untouched: [";
	PrintTasksInfo(untouched_tasks);
	std::cout << "] ";

	std::cout << std::endl;
}

