//
// Created by ilya on 26.08.2019.
//

#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include <sstream>

using namespace std;
/*
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;
*/
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
  try {
    return data_.at(person);
  }
  catch (exception& ex) {
    std::cout << "Person are not exist.\ne.what(): " << ex.what() << std::endl;
  }
}

void TeamTasks::AddNewTask(const string &person) {
  data_[person][TaskStatus::NEW] += 1;
}

tuple<TasksInfo, TasksInfo> TeamTasks::PerformPersonTasks(const string &person, int task_count) {

  TasksInfo bufer = data_[person];
  TasksInfo changed, notchanged = bufer;

  int not_end_tasks = 0;
  if (bufer.count(TaskStatus::NEW)) {
    not_end_tasks += bufer.at(TaskStatus::NEW);
  }
  if (bufer.count(TaskStatus::IN_PROGRESS)) {
    not_end_tasks += bufer.at(TaskStatus::IN_PROGRESS);
  }
  if (bufer.count(TaskStatus::TESTING)) {
    not_end_tasks += bufer.at(TaskStatus::TESTING);
  }

  if (task_count > not_end_tasks) {
    task_count = not_end_tasks;
  }

  notchanged = bufer;
  if (notchanged.count(TaskStatus::DONE)) {
    notchanged.erase(TaskStatus::DONE);
  }

  for (int i = 0; i < 3; ++i) {
    if (notchanged.count(TaskStatus(i))) {
      while (task_count && notchanged.at(TaskStatus(i))) {
        task_count--;
        bufer.at(TaskStatus(i)) -= 1;
        bufer[TaskStatus(i + 1)] += 1;

        // Стаус оставшихся задач
        notchanged.at(TaskStatus(i)) -= 1;
        // Статус обновленных задач
        changed[TaskStatus(i+1)] += 1;
      }
      if (notchanged.at(TaskStatus(i)) == 0) {
        notchanged.erase(TaskStatus(i));
      }
      if (bufer.count(TaskStatus(i))) {
        if (bufer.at(TaskStatus(i)) == 0) {
          bufer.erase(TaskStatus(i));
        }
      }
    }
  }

  data_[person] = bufer;
  return tuple(changed, notchanged);
}

/*
void PrintTasksInfo(TasksInfo tasks_info) {


  if (tasks_info.count(TaskStatus::NEW)) {
    std::cout << "NEW: " << tasks_info.at(TaskStatus::NEW) << " ";
  }
  if (tasks_info.count(TaskStatus::IN_PROGRESS)) {
    std::cout << "IN_PROGRESS: " << tasks_info.at(TaskStatus::IN_PROGRESS) << " ";
  }
  if (tasks_info.count(TaskStatus::TESTING)) {
    std::cout << "TESTING: " << tasks_info.at(TaskStatus::TESTING) << " ";
  }
  if (tasks_info.count(TaskStatus::DONE)) {
    std::cout << "DONE: " << tasks_info.at(TaskStatus::DONE) << " ";
  }


  std::cout << "NEW: " << tasks_info[TaskStatus::NEW] << " ";
  std::cout << "IN_PROGRESS: " << tasks_info[TaskStatus::IN_PROGRESS] << " ";
  std::cout << "TESTING: " << tasks_info[TaskStatus::TESTING] << " ";
  std::cout << "DONE: " << tasks_info[TaskStatus::DONE] << " ";

}

void PrintTasksInfo(const TasksInfo& updated_tasks, const TasksInfo& untouched_tasks) {
  std::cout << "Updated: [";
  PrintTasksInfo(updated_tasks);
  std::cout << "] ";

  std::cout << "Untouched: [";
  PrintTasksInfo(untouched_tasks);
  std::cout << "] ";

  std::cout << std::endl;
}
*/
