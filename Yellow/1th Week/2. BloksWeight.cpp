//
// Created by ilya on 26.08.2019.
//

#include <vector>
#include <iostream>

int main() {
  int N, R;
  std::cin >> N >> R;
  uint64_t result_weight = 0;
  for (int i = 0; i < N; ++i) {
    int W, H, D;
    std::cin >> W >> H >> D;
    uint64_t V = W*H*D;
    uint64_t weight = V * R;
    result_weight += weight;
  }
  std::cout << result_weight << std::endl;
  return 0;
}
