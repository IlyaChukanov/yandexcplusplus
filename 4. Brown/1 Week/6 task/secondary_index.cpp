//#include "test_runner.h"
#include "../../../MyUtils/MyTestFramework/TestFramework.h"
//#include "../../../MyUtils/Profiler/profiler.h"
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

  Database() {
    //time_dur = TotalDuration("- Time Duration");
    //karma_dur = TotalDuration("- Karma Duration");
    //user_dur = TotalDuration("- User Duration");
    //db_dur = TotalDuration("- DB Duration");
  }

  bool Put(const Record& record) {
    auto record_id = record.id;
    if (!database_.count(record_id)) {

      auto timestamp_inserted = timestamp_range.insert(std::make_pair(record.timestamp, record.id));
      auto karma_inserted = karma_range.insert(std::make_pair(record.karma, record.id));
      users_ids[record.user].insert(record_id);
      database_[record_id] = {record, timestamp_inserted, karma_inserted};

      /*TimeStampIndex::iterator timestamp_inserted;
      KarmaIndex::iterator karma_inserted;
      {
        ADD_DURATION(time_dur);
        timestamp_inserted = timestamp_range.insert(std::make_pair(record.timestamp, record_id));
      }
      {
        ADD_DURATION(karma_dur)
        karma_inserted = karma_range.insert(std::make_pair(record.karma, record_id));
      }
      {
        ADD_DURATION(user_dur);
        users_ids[record.user].insert(record_id);
      }
      {
        ADD_DURATION(db_dur);
        database_[record_id] = {std::move(record), timestamp_inserted, karma_inserted};
      }*/
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
      users_ids[database_[id].record_.user].erase(id);
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
    for (auto i = range_begin; i != range_end && callback(database_.at(i->second).record_); ++i);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    auto range_begin = karma_range.lower_bound(low);
    auto range_end = karma_range.upper_bound(high);
    if (range_begin == karma_range.end()) return;
    bool flag = true;
    for (auto i = range_begin; i != range_end && callback(database_.at(i->second).record_); ++i);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    auto finded = users_ids.find(user);
    if (finded == users_ids.end()) return;
    auto cont = finded->second;
    for (auto id = finded->second.begin(); id != finded->second.end() && callback(database_.at(*id).record_); ++id);
  }

 private:

  using TimeStampIndex = std::multimap<int, std::string>;
  using KarmaIndex = std::multimap<int, std::string>;
  using UserIndex = std::unordered_map<std::string, std::unordered_set<std::string>>;

  struct DBRecord {
    Record record_;
    TimeStampIndex::iterator timestamp_record_;
    KarmaIndex::iterator karma_record_;
    //UserIndex::iterator user_index_;
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


/*void HighLoad() {
  Database db;

  const size_t PUT_COUNT = 300000;
  {
    LOG_DURATION("PUT TIME")
    int id = 0;
    for (size_t i = 0; i < PUT_COUNT; ++i) {
      db.Put({"id" + std::to_string(id), "Text", "Name" + std::to_string(id / 10), 1000 + id, 10 + id % 10});
      id++;
    }
  }
  const size_t ERASE_COUNT = 100000;
  {
    LOG_DURATION("ERASE TIME")
    int id = 0;
    for (size_t i = 0; i < ERASE_COUNT; ++i) {
      db.Erase("id" + std::to_string(id));
      id += 2;
    }
  }
  const size_t GET_COUNT = 100000;
  {
    LOG_DURATION("GET TIME")
    int id = 0;
    for (size_t i = 0; i < ERASE_COUNT; ++i) {
      db.GetById("id" + std::to_string(id));
      id += 3;
    }
  }

  const size_t TIMESTAMP_COUNT = 100000;
  {
    LOG_DURATION("TIMESTAMP TIME")
    for (size_t i = 0; i < ERASE_COUNT; ++i) {
      int count = 0;
      db.RangeByTimestamp(i + 1000, i + 1100, [&count](const Record&) {
        ++count;
        return true;
      });
    }
  }

  const size_t KARMA_COUNT = 100000;
  {
    LOG_DURATION("KARMA TIME")
    for (size_t i = 0; i < ERASE_COUNT; ++i) {
      int count = 0;
      db.RangeByKarma(i + 10, i + 30, [&count](const Record&) {
        ++count;
        return true;
      });
    }
  }

  const size_t NAME_COUNT = 100000;
  {
    LOG_DURATION("NAME TIME")
    std::string name = "Name";
    for (size_t i = 0; i < ERASE_COUNT; ++i) {
      int count = 0;
      db.AllByUser(name + std::to_string(i / 10), [&count](const Record&) {
        ++count;
        return true;
      });
    }
  }


}*/

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  //RUN_TEST(tr, HighLoad);
  return 0;
}
