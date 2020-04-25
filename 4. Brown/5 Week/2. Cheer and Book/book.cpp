#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
  struct PersonInfo {
    int person_before_;
    int curr_reading_;
  };
 public:
  ReadingManager() : page_for_user_(MAX_USER_COUNT_ + 1, 0),
                     users_for_page_(MAX_PAGE_COUNT_ + 1, {0, 0}),
                     user_count(0) {}
  void Read(int user_id, int page_count) {
    int curr_page = page_for_user_[user_id];
    if (curr_page != 0) {
      users_for_page_[curr_page].curr_reading_ -= 1;
      user_count--;
    }
    users_for_page_[page_count].curr_reading_ += 1;
    page_for_user_[user_id] = page_count;
    user_count++;
    UpdateData();
  }

  double Cheer(int user_id) const {
    if (page_for_user_[user_id] == 0) {
      return 0;
    }
    if (user_count == 1) {
      return 1;
    }
    const int page_count = page_for_user_[user_id];
    int count = users_for_page_[page_count].person_before_;
    if (count == user_count) {
      return 0;
    }
    return (count) * 1.0 / (user_count - 1);
  }
 private:
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGE_COUNT_ = 1'000;

  void UpdateData() {
    int last_index = 0;
    for (int i = 1; i < MAX_PAGE_COUNT_ + 1; ++i) {
      if (users_for_page_[i].curr_reading_) {
        users_for_page_[i].person_before_ = users_for_page_[last_index].curr_reading_ +
            users_for_page_[last_index].person_before_;
        last_index = i;
      }
    }
  }

  // Хранит число людей остановившихся на этой странице и число людей до этой страницы
  // Записи отсортированы по возрастанию количества страниц
  std::vector<PersonInfo> users_for_page_;
  // Для каждого человека хранит текущую страницу
  std::vector<int> page_for_user_;
  size_t user_count;
};


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}