//
// Created by ilya on 15.07.19.
//

#include <iostream>

int main()
{
    int a = 0, b = 0;
    std::cin >> a >> b;
    if (!b)
    {
        std::cout << "Impossible" << std::endl;
        return 0;
    }

    std::cout << a / b << std::endl;
    return 0;
}