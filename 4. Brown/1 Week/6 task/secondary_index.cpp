//#include "test_runner.h"
#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

// Реализуйте этот класс
class Database {
public:
  bool Put(const Record& record) {
    if (!database_.count(record.id)) {
      database_[record.id] = record;
      timestamp_range[record.timestamp] = record.id;
      karma_range[record.karma] = record.id;
      users_ids[record.user].insert(record.id);
      return true;
    }
    else {
      return false;
    }
  }

  const Record* GetById(const string& id) const {
    if (database_.count(id)) {
      return &database_.at(id);
    }
    else {
      return nullptr;
    }
  }

  bool Erase(const string& id) {
    if (database_.count(id)) {
      timestamp_range.erase(database_[id].timestamp);
      karma_range.erase(database_[id].karma);
      users_ids[database_[id].user].erase(id);
      database_.erase(id);
      return true;
    }
    else {
      return false;
    }
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    auto range_begin = timestamp_range.lower_bound(low);
    auto range_end = timestamp_range.upper_bound(high);
    if (range_begin == timestamp_range.end()) return;
    for (auto i = range_begin; i != range_end && callback(database_.at(i->second)); ++i);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    auto range_begin = karma_range.lower_bound(low);
    auto range_end = karma_range.upper_bound(high);
    if (range_begin == karma_range.end()) return;
    for (auto i = range_begin; i != range_end && callback(database_.at(i->second)); ++i);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    auto finded = users_ids.find(user);
    if (finded == users_ids.end()) return;
    auto cont = finded->second;
    for (auto id = finded->second.begin(); id != finded->second.end() && callback(database_.at(*id)); ++id);
  }

 private:
  // ID - Record
  std::unordered_map<std::string, Record> database_;
  // Timestamp - ID
  std::map<int, std::string> timestamp_range;
  // Karma - ID
  std::map<int, std::string> karma_range;
  // User - ID
  std::unordered_map<std::string, std::unordered_set<std::string>> users_ids;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "Hello there", "master", 1536107261, good_karma - 1});
  db.Put({"id3", "O>>-<", "general2", 1536107262, bad_karma + 1});
  db.Put({"id4", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(4, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
