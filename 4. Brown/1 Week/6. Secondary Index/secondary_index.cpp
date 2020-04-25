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
      auto& res = database_[record.id];
      res.record_ = record;
      res.timestamp_record_ = timestamp_range.emplace(record.timestamp, &res.record_);
      res.karma_record_ = karma_range.emplace(record.karma, &res.record_);
      res.user_record_ = users_ids.emplace(record.user, &res.record_);
      return true;
    }
    else {
      return false;
    }
  }

  const Record* GetById(const string& id) const {
    if (database_.count(id)) {
      return &database_.at(id).record_;
    }
    else {
      return nullptr;
    }
  }

  bool Erase(const string& id) {
    if (database_.count(id)) {
      timestamp_range.erase(database_[id].timestamp_record_);
      karma_range.erase(database_[id].karma_record_);
      users_ids.erase(database_[id].user_record_);
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
    for (auto id = range_begin; id != range_end && callback(*(id->second)); ++id);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    auto range_begin = karma_range.lower_bound(low);
    auto range_end = karma_range.upper_bound(high);
    if (range_begin == karma_range.end()) return;
    bool flag = true;
    for (auto id = range_begin; id != range_end && callback(*(id->second)); ++id);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    auto finded = users_ids.equal_range(user);
    if (finded.first == users_ids.end()) return;
    for (auto id = finded.first; id != finded.second && callback(*(id->second)); ++id);
  }

 private:

  using TimeStampIndex = std::multimap<int, const Record*>;
  using KarmaIndex = std::multimap<int, const Record*>;
  using UserIndex = std::multimap<std::string, const Record*>;

  struct DBRecord {
    Record record_;
    TimeStampIndex::iterator timestamp_record_;
    KarmaIndex::iterator karma_record_;
    UserIndex::iterator user_record_;
  };

  //TotalDuration time_dur, karma_dur, user_dur, db_dur;
  // ID - Record
  std::unordered_map<std::string, DBRecord> database_;
  // Timestamp - ID
  TimeStampIndex timestamp_range;
  // Karma - ID
  KarmaIndex karma_range;
  // User - ID
  UserIndex users_ids;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id11", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "Hello there", "master", 1536107261, good_karma - 1});
  db.Put({"id3", "O>>-<", "general2", 1536107262, bad_karma + 1});
  db.Put({"id4", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(5, count);
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
