//
// Created by ilya on 15.07.19.
//

#include <iostream>
#include <cmath>

int main()
{
    double a = 0, b = 0, c = 0;
    std::cin >> a >> b >> c;

    if (a == 0 && b == 0)
    {
        return 0;
    }
    double x1 = .0, x2 = .0;
    bool single = false;

    if ((a == 0 && c == 0) ||
        (b == 0 && c == 0))
    {
        x1 = .0;
        single = true;
    }
    else {
        if (a == 0)
        {
            x1 = -c / b;
            single = true;
        }
        else if (b == 0)
        {
            double sqr = -c / a;
            if (sqr < 0)
                return 0;
            x1 = sqr;
            x2 = -sqr;
            single = true;
        }
        else if (c == 0)
        {
            x1 = .0;
            x2 = -b / a;
        }
        else
        {
            double D = b * b - 4 * a * c;
            if (D > 0)
            {
                x1 = (-b + sqrt(D)) / (2.0 * a);
                x2 = (-b - sqrt(D)) / (2.0 * a);
            }
            else if (D == 0)
            {
                x1 = -b / (2.0 * a);
                single = true;
            }
            else {
                return 0;
            }
        }
    }


    if (single) std::cout << x1 << std::endl;
    else std::cout << x1 << " " << x2 << std::endl;
    return 0;
}