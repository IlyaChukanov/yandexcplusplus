//
// Created by ilya on 15.07.19.
//

#include <iostream>

int main() {
    int A, B;
    std::cin >> A >> B;

    // Basic case
    /*
    for (int i = A; i <= B; ++i) {
        if (i % 2 == 0) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
     */
    // More good case
    int i = A;
    if (A % 2 != 0) {
        i++;
    }
    for (; i <= B; i += 2) {
        std::cout << i << " ";
    }
    return 0;
}