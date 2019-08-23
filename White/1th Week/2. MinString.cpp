//
// Created by ilya on 15.07.19.
//

#include <iostream>
#include <string>

int main()
{
    std::string a, b, c;
    std::cin >> a >> b >> c;
    if (a > b)
        a = b;
    if (a > c)
        a = c;
    std::cout << a << std::endl;
    return 0;
}
