#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <iostream>
#include <tuple>


enum class TaskStatus 
{
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};


std::ostream& operator <<(std::ostream& out, const TaskStatus& status);


// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;


class TeamTasksTester;


TasksInfo operator +(const TasksInfo& lhs, const TasksInfo& rhs);


class TeamTasks 
{
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const std::string& person) const;

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const std::string& person);

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, 
		int task_count);

private:
	std::unordered_map<std::string, TasksInfo> programmers_;

	void remove_empty(TasksInfo&);

	friend class TeamTasksTester; // for testing private fields of TeamTasks
};


void TestTasksTracker();