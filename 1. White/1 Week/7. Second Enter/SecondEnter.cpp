//
// Created by ilya on 15.07.19.
//

#include <iostream>
#include <string>

int main()
{
    int count_enter = 2;
    int current_enter = 0;
    int index = 0;
    std::string str;
    std::cin >> str;
    char c_enter = 'f';
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == c_enter) {
            current_enter++;
        }
        if (current_enter == count_enter) {
            std::cout << i << std::endl;
            break;
        }
    }
    if (current_enter == 1) {
        std::cout << -1 << std::endl;
    }
    if (current_enter == 0) {
        std::cout << -2 << std::endl;
    }
    return 0;
}