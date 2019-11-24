//
// Created by ilya on 15.07.19.
//

#include <iostream>

int main() {

    int A, B;
    std::cin >> A >> B;
    while (B) {
        A %= B;
        int buff = A;
        A = B;
        B = buff;
    }

    std::cout << A << std::endl;
    return 0;
}