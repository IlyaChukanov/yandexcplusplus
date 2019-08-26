//
// Created by ilya on 26.08.2019.
//

#include <iostream>
#include <string>
#include <map>
#include <tuple>

using namespace std;

enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
 public:

  TeamTasks() = default;

  const TasksInfo& GetPersonTasksInfo(const string& person) const;

  void AddNewTask(const string& person);

  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count);

 private:
  std::map<std::string, TasksInfo> data_;
};

const TasksInfo& TeamTasks::GetPersonTasksInfo(const string &person) const {

}