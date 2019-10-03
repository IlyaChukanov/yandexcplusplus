//
// Created by ilya on 03.10.2019.
//

#include <iostream>
#include <vector>
#include <list>
#include <map>

class SportQueue {
 public:
  void Add(int32_t id, int32_t id_after) {
    auto find = position_.find(id_after);
    if (find == position_.end()) {
      queue_.push_back(id);
      position_[id] = prev(queue_.end());
    }
    else {
      auto ins = queue_.insert(find->second, id);
      position_[id] = ins;
    }
  }

  std::vector<int32_t> GetQueue() {
    return {queue_.begin(), queue_.end()};
  }

 private:
  std::list<int32_t > queue_;
  std::map<int32_t, std::list<int32_t>::iterator> position_;
};

int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  int32_t Q{0};
  std::cin >> Q;
  SportQueue sq;
  for (int32_t i = 0; i < Q; ++i) {
    int32_t id{0}, id_after{0};
    std::cin >> id >> id_after;
    sq.Add(id, id_after);
  }
  auto res = sq.GetQueue();
  for (const auto& i : res) {
    std::cout << i << " ";
  }
  return 0;
}