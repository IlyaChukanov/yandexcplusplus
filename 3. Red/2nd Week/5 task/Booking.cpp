//
// Created by ilya on 01.10.2019.
//

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>

struct Record {
  Record()= default;
  int64_t time = 0;
  std::string name_hotel;
  int32_t client_id = 0;
  int32_t rooms_count = 0;
};

struct PointToEvent {
  std::list<Record>::iterator event;
  int32_t rooms;
};

class Hotel {
 public:
  Hotel() : clients_(), rooms_(0) {}

  void BookClient(int32_t client_id, int32_t rooms, std::list<Record>::iterator iterator) {
    rooms_ += rooms;
    auto& ref = clients_[client_id];
    ref.event = iterator;
    ref.rooms = rooms;
  }

  void RemoveClient(int32_t client_id) {
    auto& ref = clients_[client_id];
    rooms_ -= ref.event;
    clients_.erase(client_id);
  }

  bool CheckClient(int32_t client_id) {
    return clients_.count(client_id);
  }

  bool CheckAndRemove(int32_t client_id) {

  }

  int32_t GetClients() const {
    return clients_.size();
  }

  int32_t GerRooms() const {
    return rooms_;
  }

 private:
  std::map<int32_t, PointToEvent> clients_;
  int32_t rooms_;
};



class BookingManager {
 public:
  BookingManager() = default;
  void Book(int64_t time, const std::string& hotel_name, int32_t client_id, uint32_t room_count) {
    if (CheckRecordTime(time)) {
      Record& r_ref = records_.back();
      Hotel& h_ref = r_ref.record_data[hotel_name];
      if (h_ref.CheckClient(client_id)) {
        h_ref.RemoveClient(client_id);
      }
      h_ref.BookClient(client_id, room_count);
    }
    else {
      Record r;
      r.time = time;
      Hotel& h_ref = r.record_data[hotel_name];
      h_ref.BookClient(client_id, room_count);
      records_.push_back(r);
    }
    Update();
  }

  int32_t Clients(const std::string& hotel_name) const {
    int32_t result = 0;
    for (const auto& record : records_) {
      if (record.record_data.count(hotel_name)) {
        result += record.record_data.at(hotel_name).GetClients();
      }
    }
    return result;
  }

  uint32_t Rooms(const std::string& hotel_name) {
    int32_t result = 0;
    for (const auto& record : records_) {
      if (record.record_data.count(hotel_name)) {
        result += record.record_data.at(hotel_name).GerRooms();
      }
    }
    return result;
  }

 private:
  static const int PERIOD_TIME = 86400;
  std::list<Record> records_;
  std::map<std::string, Hotel> hotels_;

  void Update() {
    for (auto record = records_.begin(); record != records_.end(); ++record) {
      if (!CheckRecordTime(record->time)) {
        record = records_.erase(records_.begin());
      }
      else {
        break;
      }
    }
  }

  void UpdateData() {
    auto for_delete = records_.front();
  }

  bool CheckRecordTime(int32_t time) {
    int32_t current_time = records_.back().time;
    return ((current_time - PERIOD_TIME) < time) && (time <= current_time);
  }

};

int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  BookingManager manager;

  int query_count;
  std::cin >> query_count;

  for (uint32_t query_id = 0; query_id < query_count; ++query_id) {
    std::string query_type;
    std::cin >> query_type;
    if (query_type == "BOOK") {
      int64_t time, client_id, room_count;
      std::string hotel_name;
      std::cin >> time >> hotel_name >> client_id >> room_count;
      manager.Book(time, hotel_name, client_id, room_count);
    } else if (query_type == "CLIENTS") {
      std::string hotel_name;
      std::cin >> hotel_name;
      std::cout << manager.Clients(hotel_name) << "\n";
    }
    else if (query_type == "ROOMS") {
      std::string hotel_name;
      std::cin >> hotel_name;
      std::cout << manager.Rooms(hotel_name) << "\n";
    }
  }
  return 0;
}