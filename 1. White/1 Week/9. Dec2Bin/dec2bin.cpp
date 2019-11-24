//
// Created by ilya on 15.07.19.
//

#include <iostream>
#include <vector>

int main() {
    int N;
    std::cin >> N;
    std::vector<int> result;
    while(N > 1) {
        int res = N % 2;
        N /= 2;
        result.push_back(res);
    }
    result.push_back(1);
    for (auto i = result.rbegin(); i != result.rend(); ++i) {
        std::cout << *i;
    }
    return 0;
}