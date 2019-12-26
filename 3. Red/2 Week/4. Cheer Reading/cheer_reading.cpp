#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

#include <map>
#include <set>

using namespace std;

class ReadingManager {
public:
  ReadingManager() : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        sorted_users_(),
        user_positions_(MAX_USER_COUNT_ + 1, -1) {

  }

  // O(N)
  void Read(int user_id, int page_count) {
    // O(1)
    if (user_page_counts_[user_id] == 0) {
      AddUser(user_id);
    }
    user_page_counts_[user_id] = page_count;
    int& position = user_positions_[user_id];
    // O(N)
    while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
      SwapUsers(position, position - 1);
    }
  }

  double Cheer(int user_id) const {
    if (user_page_counts_[user_id] == 0) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }
    const int page_count = user_page_counts_[user_id];
    int position = user_positions_[user_id];
    // O(N)
    while (position < user_count && user_page_counts_[sorted_users_[position]] == page_count) {
      ++position;
    }
    if (position == user_count) {
        return 0;
    }
    return (user_count - position) * 1.0 / (user_count - 1);
  }

private:
  static const int MAX_USER_COUNT_ = 100'000;

  vector<int> user_page_counts_;
  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
  vector<int> user_positions_; // позиции в векторе sorted_users_

  // O(1)
  int GetUserCount() const {
    return sorted_users_.size();
  }

  // O(1)
  void AddUser(int user_id) {
    sorted_users_.push_back(user_id);
    user_positions_[user_id] = sorted_users_.size() - 1;
  }

  // O(1)
  void SwapUsers(int lhs_position, int rhs_position) {
    const int lhs_id = sorted_users_[lhs_position];
    const int rhs_id = sorted_users_[rhs_position];
    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
  }
};

class MyReadingManager {
  struct PersonInfo {
    int person_before_;
    int curr_reading_;
  };
 public:
  MyReadingManager() : page_for_user_(MAX_USER_COUNT_ + 1, 0), users_for_page_(MAX_PAGE_COUNT_ + 1, {0, 0}), user_count(0) {}
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

  MyReadingManager manager;

  int query_count;
  cin >> query_count;

  // O(Q)
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      // O(N)
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      // O(N)
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}