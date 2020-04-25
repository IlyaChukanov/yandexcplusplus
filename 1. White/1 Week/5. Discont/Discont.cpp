//
// Created by ilya on 15.07.19.
//

#include <iostream>

int main() {
    double A, X, B, Y, N;
    std::cin >> N >> A >> B >> X >> Y;

    if (N > B) {
        N -= (N/100) * Y;
    }
    else if (N > A) {
        N -= (N/100) * X;
    }
    std::cout << N << std::endl;
    return 0;
}